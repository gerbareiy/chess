module;
#include "Envelope.pb.h"
#include <optional>
#include <string>
#include <utility>
export module Chess.Network.GameSession;
import Chess.Core.ePieceColor;
import Chess.Core.Move;
import Chess.Network.BoardSnapshot;
import Chess.Network.Chessboard;
import Chess.Network.ClientConnection;
import Chess.Network.eSessionEvent;
import Chess.Network.Move;
import Chess.Network.PieceColorAndType;
import Chess.Network.ServerMessage;
import Chess.Network.Session;

namespace Chess::Network
{
    // Owns the client side of the find-game/game-started handshake and the wire
    // (Envelope/protobuf) format, so callers only ever see domain types.
    export class GameSession
    {
    public:
        static GameSession Connect(const std::string& host, unsigned short port)
        {
            auto connection = ClientConnection::Connect(host, port);

            chess::proto::Envelope findGame;
            findGame.mutable_find_game();
            connection.SendBytes(findGame.SerializeAsString());

            chess::proto::Envelope started;
            started.ParseFromString(connection.ReceiveBytes());

            const auto myColor = PieceColorAndType::FromProto(started.game_started().your_color());
            auto       board   = ToSnapshot(started.game_started().board());

            return GameSession(std::move(connection), myColor, std::move(board));
        }

        Core::ePieceColor GetMyColor() const
        {
            return myColor_;
        }

        const BoardSnapshot& GetInitialBoard() const
        {
            return initialBoard_;
        }

        void SendMove(const Core::Move& move)
        {
            chess::proto::Envelope envelope;
            *envelope.mutable_move() = Move::ToProto(move);
            connection_.SendBytes(envelope.SerializeAsString());
        }

        ServerMessage ReceiveNext()
        {
            chess::proto::Envelope envelope;
            envelope.ParseFromString(connection_.ReceiveBytes());

            switch (envelope.payload_case())
            {
            case chess::proto::Envelope::kMove:
                return ServerMessage{ eSessionEvent::OPPONENT_MOVED, Move::FromProto(envelope.move()), std::nullopt, std::nullopt };
            case chess::proto::Envelope::kBoardSync:
                return ServerMessage{ eSessionEvent::BOARD_SYNCED, std::nullopt, ToSnapshot(envelope.board_sync()), std::nullopt };
            case chess::proto::Envelope::kBoardCheck:
                return ServerMessage{ eSessionEvent::BOARD_CHECKED, std::nullopt, ToSnapshot(envelope.board_check()), std::nullopt };
            case chess::proto::Envelope::kGameOver:
                return ServerMessage{
                    eSessionEvent::GAME_OVER, std::nullopt, ToSnapshot(envelope.game_over().board()), Session::FromProto(envelope.game_over().state())
                };
            default:
                return ServerMessage{ eSessionEvent::UNKNOWN, std::nullopt, std::nullopt, std::nullopt };
            }
        }

        void Close()
        {
            connection_.Close();
        }

    private:
        ClientConnection  connection_;
        Core::ePieceColor myColor_;
        BoardSnapshot     initialBoard_;

        static BoardSnapshot ToSnapshot(const chess::proto::Chessboard& board)
        {
            return BoardSnapshot{ Chessboard::FromProto(board), PieceColorAndType::FromProto(board.side_to_move()), board.ply() };
        }

        GameSession(ClientConnection connection, Core::ePieceColor myColor, BoardSnapshot initialBoard)
            : connection_(std::move(connection))
            , myColor_(myColor)
            , initialBoard_(std::move(initialBoard))
        {
        }
    };
} // namespace Chess::Network
