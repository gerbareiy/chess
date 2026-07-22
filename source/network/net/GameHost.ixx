module;
#include "Envelope.pb.h"
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>
export module Chess.Net.GameHost;
import Chess.Chessboard;
import Chess.ChessboardFactory;
import Chess.eGameState;
import Chess.ePieceColor;
import Chess.FixedPromoter;
import Chess.GameStateChecker;
import Chess.Move;
import Chess.Net.Socket;
import Chess.Piece;
import Chess.PieceDirector;
import Chess.Proto.Chessboard;
import Chess.Proto.Move;
import Chess.Proto.PieceColorAndType;
import Chess.Proto.Session;

namespace Chess::Net
{
    export class GameHost
    {
        static constexpr uint32_t BOARD_CHECK_PERIOD = 10;

        static chess::proto::Chessboard BoardToProto(const std::shared_ptr<Chess::Chessboard>& board, uint32_t ply)
        {
            return Chess::Proto::Chessboard::ToProto(board->GetPieceDirector()->GetPiecesOnBoard(), board->GetSideToMove(), ply);
        }

        static void SendGameStarted(
            ServerSocket&                             socket,
            const std::string&                        identity,
            Chess::ePieceColor                        color,
            const std::shared_ptr<Chess::Chessboard>& board,
            uint32_t                                  ply)
        {
            chess::proto::Envelope envelope;
            auto*                  started = envelope.mutable_game_started();
            started->set_your_color(Chess::Proto::PieceColorAndType::ToProto(color));
            *started->mutable_board() = BoardToProto(board, ply);
            socket.SendFrame(identity, envelope.SerializeAsString());
        }

        static void SendBoardSync(ServerSocket& socket, const std::string& identity, const std::shared_ptr<Chess::Chessboard>& board, uint32_t ply)
        {
            chess::proto::Envelope envelope;
            *envelope.mutable_board_sync() = BoardToProto(board, ply);
            socket.SendFrame(identity, envelope.SerializeAsString());
        }

        static void SendBoardCheck(ServerSocket& socket, const std::string& identity, const std::shared_ptr<Chess::Chessboard>& board, uint32_t ply)
        {
            chess::proto::Envelope envelope;
            *envelope.mutable_board_check() = BoardToProto(board, ply);
            socket.SendFrame(identity, envelope.SerializeAsString());
        }

        static void SendGameOver(
            ServerSocket& socket, const std::string& identity, Chess::eGameState state, const std::shared_ptr<Chess::Chessboard>& board, uint32_t ply)
        {
            chess::proto::Envelope envelope;
            auto*                  over = envelope.mutable_game_over();
            over->set_state(Chess::Proto::Session::ToProto(state));
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
            ServerSocket& socket, const std::string& whiteId, const std::string& blackId, const std::shared_ptr<Chess::Chessboard>& board)
        {
            Chess::GameStateChecker checker;
            const std::string*      current  = &whiteId;
            const std::string*      opponent = &blackId;
            uint32_t                ply      = 0;

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

                const auto [from, to, promotion] = Chess::Proto::Move::FromProto(incoming.move());
                const bool valid = board->TrySelectPiece(from) && board->TryMovePiece(to, std::make_shared<Chess::FixedPromoter>(promotion));

                auto state = Chess::eGameState::PLAYING;
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

                    if (state == Chess::eGameState::CHECKMATE || state == Chess::eGameState::DRAW)
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

    public:
        static void HostSingleMatch(ServerSocket& socket, std::vector<std::shared_ptr<Chess::Piece>> pieces)
        {
            const auto [whiteId, blackId] = WaitForPlayers(socket);

            const auto board = Chess::ChessboardFactory::Create(std::move(pieces), Chess::ePieceColor::WHITE);

            SendGameStarted(socket, whiteId, Chess::ePieceColor::WHITE, board, 0);
            SendGameStarted(socket, blackId, Chess::ePieceColor::BLACK, board, 0);

            RunMatch(socket, whiteId, blackId, board);
        }
    };
} // namespace Chess::Net
