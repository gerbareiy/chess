module;
#include "Envelope.pb.h"
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <deque>
#include <functional>
#include <memory>
#include <optional>
#include <ranges>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
export module Chess.Network.GameHost;
import Chess.Core.Chessboard;
import Chess.Core.ChessboardFactory;
import Chess.Core.eGameState;
import Chess.Core.ePieceColor;
import Chess.Core.Move;
import Chess.Core.Piece;
import Chess.Core.PieceDirector;
import Chess.Network.Chessboard;
import Chess.Network.ConnectionError;
import Chess.Network.Frame;
import Chess.Network.Match;
import Chess.Network.Move;
import Chess.Network.PieceColorAndType;
import Chess.Network.ServerSocket;
import Chess.Network.Session;

namespace Chess::Network
{
    // Serves any number of games at once from a single receive loop: players asking for a
    // game are queued and paired as they arrive, and every later message is routed to the
    // match its sender belongs to.
    export class GameHost
    {
    public:
        GameHost(ServerSocket& socket, std::function<std::vector<std::shared_ptr<Core::Piece>>()> createPieces)
            : socket_(socket)
            , createPieces_(std::move(createPieces))
        {
        }

        // Serves forever, until the socket itself fails.
        void Run()
        {
            while (TryHandleNextMessage(std::chrono::milliseconds(-1)))
            {
            }
        }

        // Serves until no message arrives within idleTimeout, so a caller that knows the
        // traffic has stopped can get control back instead of blocking forever.
        void RunUntilIdle(std::chrono::milliseconds idleTimeout)
        {
            while (TryHandleNextMessage(idleTimeout))
            {
            }
        }

        size_t GetActiveMatchesCount() const
        {
            return matches_.size();
        }

        size_t GetStartedMatchesCount() const
        {
            return startedMatchesCount_;
        }

        size_t GetWaitingPlayersCount() const
        {
            return waiting_.size();
        }

        bool TryHandleNextMessage(std::chrono::milliseconds timeout)
        {
            auto frame = std::optional<Frame>();
            try
            {
                frame = socket_.TryReceiveFrame(timeout);
            }
            catch (const ConnectionError&)
            {
                return false;
            }

            if (!frame.has_value())
            {
                return false;
            }

            if (frame.value().disconnected)
            {
                HandleDisconnect(frame.value().identity);
                return true;
            }

            chess::proto::Envelope incoming;
            if (!incoming.ParseFromString(frame.value().payload))
            {
                return true;
            }

            switch (incoming.payload_case())
            {
            case chess::proto::Envelope::kFindGame:
                HandleFindGame(frame.value().identity);
                break;
            case chess::proto::Envelope::kMove:
                HandleMove(frame.value().identity, incoming.move());
                break;
            default:
                break;
            }
            return true;
        }

    private:
        static constexpr uint32_t BOARD_CHECK_PERIOD = 10;

        ServerSocket&                                              socket_;
        std::function<std::vector<std::shared_ptr<Core::Piece>>()> createPieces_;
        std::deque<std::string>                                    waiting_;
        std::unordered_map<std::string, std::shared_ptr<Match>>    matches_;
        size_t                                                     startedMatchesCount_ = 0;

        static chess::proto::Chessboard BoardToProto(const std::shared_ptr<Core::Chessboard>& board, uint32_t ply)
        {
            return Chessboard::ToProto(board->GetPieceDirector()->GetPiecesOnBoard(), board->GetSideToMove(), ply);
        }

        void HandleFindGame(const std::string& identity)
        {
            if (matches_.contains(identity) || std::ranges::find(waiting_, identity) != waiting_.end())
            {
                return;
            }

            if (waiting_.empty())
            {
                waiting_.push_back(identity);
                return;
            }

            const auto whiteId = waiting_.front();
            waiting_.pop_front();

            auto match = std::make_shared<Match>(whiteId, identity, Core::ChessboardFactory::Create(createPieces_(), Core::ePieceColor::WHITE));
            matches_.emplace(whiteId, match);
            matches_.emplace(identity, match);
            ++startedMatchesCount_;

            if (!TrySendGameStarted(*match, whiteId) || !TrySendGameStarted(*match, identity))
            {
                // One of the pair vanished between asking for a game and being given one.
                DropMatch(*match);
            }
        }

        void HandleMove(const std::string& identity, const chess::proto::Move& move)
        {
            const auto found = matches_.find(identity);
            if (found == matches_.end())
            {
                return;
            }

            // Held by value: ending a match erases it from the map, which destroys it, so
            // nothing may outlive that as a reference into the match itself.
            const auto matchOwner = found->second;
            auto&      match      = *matchOwner;
            const auto opponent   = match.GetOtherPlayerId(identity);

            if (identity != match.GetCurrentId())
            {
                // A message from the player who isn't on move — ignore it.
                return;
            }

            if (!match.TryApplyMove(Move::FromProto(move)))
            {
                TrySendBoardSync(match, identity);
                return;
            }

            chess::proto::Envelope forwarded;
            *forwarded.mutable_move() = move;
            if (!TrySend(opponent, forwarded))
            {
                EndMatchBecausePlayerLeft(match, opponent);
                return;
            }

            if (match.GetIsFinished())
            {
                TrySendGameOver(match, match.GetWhiteId());
                TrySendGameOver(match, match.GetBlackId());
                DropMatch(match);
                return;
            }

            if (match.GetPly() % BOARD_CHECK_PERIOD == 0)
            {
                TrySendBoardCheck(match, match.GetWhiteId());
                TrySendBoardCheck(match, match.GetBlackId());
            }
        }

        void HandleDisconnect(const std::string& identity)
        {
            const auto waitingPlace = std::ranges::find(waiting_, identity);
            if (waitingPlace != waiting_.end())
            {
                waiting_.erase(waitingPlace);
            }

            const auto found = matches_.find(identity);
            if (found == matches_.end())
            {
                return;
            }

            const auto matchOwner = found->second;
            EndMatchBecausePlayerLeft(*matchOwner, identity);
        }

        void EndMatchBecausePlayerLeft(const Match& match, const std::string& leaverId)
        {
            const auto survivorId = match.GetOtherPlayerId(leaverId);

            DropMatch(match);

            chess::proto::Envelope envelope;
            envelope.mutable_opponent_left();
            TrySend(survivorId, envelope);
        }

        void DropMatch(const Match& match)
        {
            const auto whiteId = match.GetWhiteId();
            const auto blackId = match.GetBlackId();
            matches_.erase(whiteId);
            matches_.erase(blackId);
        }

        bool TrySend(const std::string& identity, const chess::proto::Envelope& envelope)
        {
            try
            {
                socket_.SendFrame(identity, envelope.SerializeAsString());
            }
            catch (const ConnectionError&)
            {
                return false;
            }
            return true;
        }

        bool TrySendGameStarted(const Match& match, const std::string& identity)
        {
            chess::proto::Envelope envelope;
            auto*                  started = envelope.mutable_game_started();
            started->set_your_color(PieceColorAndType::ToProto(match.GetColorOf(identity)));
            *started->mutable_board() = BoardToProto(match.GetBoard(), match.GetPly());
            return TrySend(identity, envelope);
        }

        bool TrySendBoardSync(const Match& match, const std::string& identity)
        {
            chess::proto::Envelope envelope;
            *envelope.mutable_board_sync() = BoardToProto(match.GetBoard(), match.GetPly());
            return TrySend(identity, envelope);
        }

        bool TrySendBoardCheck(const Match& match, const std::string& identity)
        {
            chess::proto::Envelope envelope;
            *envelope.mutable_board_check() = BoardToProto(match.GetBoard(), match.GetPly());
            return TrySend(identity, envelope);
        }

        bool TrySendGameOver(const Match& match, const std::string& identity)
        {
            chess::proto::Envelope envelope;
            auto*                  over = envelope.mutable_game_over();
            over->set_state(Session::ToProto(match.GetState()));
            *over->mutable_board() = BoardToProto(match.GetBoard(), match.GetPly());
            return TrySend(identity, envelope);
        }
    };
} // namespace Chess::Network
