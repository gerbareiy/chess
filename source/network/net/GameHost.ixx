module;
#include "Envelope.pb.h"
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>
export module Chess.Network.GameHost;
import Chess.Core.Chessboard;
import Chess.Core.ChessboardFactory;
import Chess.Core.eGameState;
import Chess.Core.ePieceColor;
import Chess.Core.FixedPromoter;
import Chess.Core.GameStateChecker;
import Chess.Core.Move;
import Chess.Core.Piece;
import Chess.Core.PieceDirector;
import Chess.Network.Chessboard;
import Chess.Network.ConnectionError;
import Chess.Network.Frame;
import Chess.Network.Move;
import Chess.Network.PieceColorAndType;
import Chess.Network.ServerSocket;
import Chess.Network.Session;

namespace Chess::Network
{
    export class GameHost
    {
    public:
        static void HostSingleMatch(ServerSocket& socket, std::vector<std::shared_ptr<Chess::Core::Piece>> pieces)
        {
            const auto [whiteId, blackId] = WaitForPlayers(socket);

            const auto board = Chess::Core::ChessboardFactory::Create(std::move(pieces), Chess::Core::ePieceColor::WHITE);

            SendGameStarted(socket, whiteId, Chess::Core::ePieceColor::WHITE, board, 0);
            SendGameStarted(socket, blackId, Chess::Core::ePieceColor::BLACK, board, 0);

            RunMatch(socket, whiteId, blackId, board);
        }

    private:
        static constexpr uint32_t BOARD_CHECK_PERIOD = 10;

        static chess::proto::Chessboard BoardToProto(const std::shared_ptr<Chess::Core::Chessboard>& board, uint32_t ply)
        {
            return Chess::Network::Chessboard::ToProto(board->GetPieceDirector()->GetPiecesOnBoard(), board->GetSideToMove(), ply);
        }

        static void SendGameStarted(
            ServerSocket&                                   socket,
            const std::string&                              identity,
            Chess::Core::ePieceColor                        color,
            const std::shared_ptr<Chess::Core::Chessboard>& board,
            uint32_t                                        ply)
        {
            chess::proto::Envelope envelope;
            auto*                  started = envelope.mutable_game_started();
            started->set_your_color(Chess::Network::PieceColorAndType::ToProto(color));
            *started->mutable_board() = BoardToProto(board, ply);
            socket.SendFrame(identity, envelope.SerializeAsString());
        }

        static void SendBoardSync(
            ServerSocket& socket, const std::string& identity, const std::shared_ptr<Chess::Core::Chessboard>& board, uint32_t ply)
        {
            chess::proto::Envelope envelope;
            *envelope.mutable_board_sync() = BoardToProto(board, ply);
            socket.SendFrame(identity, envelope.SerializeAsString());
        }

        static void SendBoardCheck(
            ServerSocket& socket, const std::string& identity, const std::shared_ptr<Chess::Core::Chessboard>& board, uint32_t ply)
        {
            chess::proto::Envelope envelope;
            *envelope.mutable_board_check() = BoardToProto(board, ply);
            socket.SendFrame(identity, envelope.SerializeAsString());
        }

        static void SendGameOver(
            ServerSocket&                                   socket,
            const std::string&                              identity,
            Chess::Core::eGameState                         state,
            const std::shared_ptr<Chess::Core::Chessboard>& board,
            uint32_t                                        ply)
        {
            chess::proto::Envelope envelope;
            auto*                  over = envelope.mutable_game_over();
            over->set_state(Chess::Network::Session::ToProto(state));
            *over->mutable_board() = BoardToProto(board, ply);
            socket.SendFrame(identity, envelope.SerializeAsString());
        }

        // Blocks until two distinct client identities have sent find_game. Disconnect
        // notifications (empty payload) and anything else received meanwhile are ignored.
        static std::pair<std::string, std::string> WaitForPlayers(ServerSocket& socket)
        {
            std::string white;
            while (white.empty())
            {
                const auto             frame = socket.ReceiveFrame();
                chess::proto::Envelope envelope;
                if (!frame.disconnected && envelope.ParseFromString(frame.payload) && envelope.payload_case() == chess::proto::Envelope::kFindGame)
                {
                    white = frame.identity;
                }
            }

            std::string black;
            while (black.empty())
            {
                const auto             frame = socket.ReceiveFrame();
                chess::proto::Envelope envelope;
                if (frame.identity != white && !frame.disconnected && envelope.ParseFromString(frame.payload)
                    && envelope.payload_case() == chess::proto::Envelope::kFindGame)
                {
                    black = frame.identity;
                }
            }

            return { white, black };
        }

        static void RunMatch(
            ServerSocket& socket, const std::string& whiteId, const std::string& blackId, const std::shared_ptr<Chess::Core::Chessboard>& board)
        {
            Chess::Core::GameStateChecker checker;
            const std::string*            current  = &whiteId;
            const std::string*            opponent = &blackId;
            uint32_t                      ply      = 0;

            while (true)
            {
                Frame frame;
                try
                {
                    frame = socket.ReceiveFrame();
                }
                catch (const ConnectionError&)
                {
                    // a player disconnected, end the match.
                    return;
                }

                if (frame.disconnected && (frame.identity == whiteId || frame.identity == blackId))
                {
                    // a player disconnected, end the match.
                    return;
                }

                if (frame.identity != *current)
                {
                    // message from the player who isn't on move — ignore it.
                    continue;
                }

                chess::proto::Envelope incoming;
                if (!incoming.ParseFromString(frame.payload) || incoming.payload_case() != chess::proto::Envelope::kMove)
                {
                    continue;
                }

                const auto [from, to, promotion] = Chess::Network::Move::FromProto(incoming.move());
                const bool valid = board->TrySelectPiece(from) && board->TryMovePiece(to, std::make_shared<Chess::Core::FixedPromoter>(promotion));

                auto state = Chess::Core::eGameState::PLAYING;
                if (valid)
                {
                    ++ply;
                    state = checker.Calculate(board);
                }

                try
                {
                    if (!valid)
                    {
                        SendBoardSync(socket, *current, board, ply);
                        continue;
                    }

                    chess::proto::Envelope forwarded;
                    *forwarded.mutable_move() = incoming.move();
                    socket.SendFrame(*opponent, forwarded.SerializeAsString());

                    if (state == Chess::Core::eGameState::CHECKMATE || state == Chess::Core::eGameState::DRAW)
                    {
                        SendGameOver(socket, whiteId, state, board, ply);
                        SendGameOver(socket, blackId, state, board, ply);
                        return;
                    }

                    if (ply % BOARD_CHECK_PERIOD == 0)
                    {
                        SendBoardCheck(socket, whiteId, board, ply);
                        SendBoardCheck(socket, blackId, board, ply);
                    }
                }
                catch (const ConnectionError&)
                {
                    // one of the players disconnected, end the match.
                    return;
                }

                std::swap(current, opponent);
            }
        }
    };
} // namespace Chess::Network
