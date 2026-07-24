module;
#include "Envelope.pb.h"
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>
export module Chess.Net.GameSession;
import Chess.eGameState;
import Chess.ePieceColor;
import Chess.Move;
import Chess.Net.ClientConnection;
import Chess.Piece;
import Chess.Proto.Chessboard;
import Chess.Proto.Move;
import Chess.Proto.PieceColorAndType;
import Chess.Proto.Session;

namespace Chess::Net
{
    export struct BoardSnapshot
    {
        std::vector<std::shared_ptr<Chess::Piece>> pieces;
        Chess::ePieceColor                         sideToMove = Chess::ePieceColor::NONE;
        uint32_t                                   ply        = 0;
    };

    export enum class eSessionEvent
    {
        OPPONENT_MOVED,
        BOARD_SYNCED,
        BOARD_CHECKED,
        GAME_OVER,
        UNKNOWN
    };

    export struct ServerMessage
    {
        eSessionEvent                    event;
        std::optional<Chess::Move>       move;
        std::optional<BoardSnapshot>     board;
        std::optional<Chess::eGameState> finalState;
    };

    // Owns the client side of the find-game/game-started handshake and the wire
    // (Envelope/protobuf) format, so callers only ever see domain types.
    export class GameSession
    {
        ClientConnection   m_connection;
        Chess::ePieceColor m_myColor;
        BoardSnapshot      m_initialBoard;

        static BoardSnapshot ToSnapshot(const chess::proto::Chessboard& board)
        {
            return BoardSnapshot{ Proto::Chessboard::FromProto(board), Proto::PieceColorAndType::FromProto(board.side_to_move()), board.ply() };
        }

        GameSession(ClientConnection connection, Chess::ePieceColor myColor, BoardSnapshot initialBoard)
            : m_connection(std::move(connection))
            , m_myColor(myColor)
            , m_initialBoard(std::move(initialBoard))
        {
        }

    public:
        static GameSession Connect(const std::string& host, unsigned short port)
        {
            auto connection = ClientConnection::Connect(host, port);

            chess::proto::Envelope findGame;
            findGame.mutable_find_game();
            connection.SendBytes(findGame.SerializeAsString());

            chess::proto::Envelope started;
            started.ParseFromString(connection.ReceiveBytes());

            auto myColor = Proto::PieceColorAndType::FromProto(started.game_started().your_color());
            auto board   = ToSnapshot(started.game_started().board());

            return GameSession(std::move(connection), myColor, std::move(board));
        }

        Chess::ePieceColor GetMyColor() const
        {
            return m_myColor;
        }

        const BoardSnapshot& GetInitialBoard() const
        {
            return m_initialBoard;
        }

        void SendMove(const Chess::Move& move)
        {
            chess::proto::Envelope envelope;
            *envelope.mutable_move() = Proto::Move::ToProto(move);
            m_connection.SendBytes(envelope.SerializeAsString());
        }

        ServerMessage ReceiveNext()
        {
            chess::proto::Envelope envelope;
            envelope.ParseFromString(m_connection.ReceiveBytes());

            switch (envelope.payload_case())
            {
            case chess::proto::Envelope::kMove:
                return ServerMessage{ eSessionEvent::OPPONENT_MOVED, Proto::Move::FromProto(envelope.move()), std::nullopt, std::nullopt };
            case chess::proto::Envelope::kBoardSync:
                return ServerMessage{ eSessionEvent::BOARD_SYNCED, std::nullopt, ToSnapshot(envelope.board_sync()), std::nullopt };
            case chess::proto::Envelope::kBoardCheck:
                return ServerMessage{ eSessionEvent::BOARD_CHECKED, std::nullopt, ToSnapshot(envelope.board_check()), std::nullopt };
            case chess::proto::Envelope::kGameOver:
                return ServerMessage{ eSessionEvent::GAME_OVER,
                                      std::nullopt,
                                      ToSnapshot(envelope.game_over().board()),
                                      Proto::Session::FromProto(envelope.game_over().state()) };
            default:
                return ServerMessage{ eSessionEvent::UNKNOWN, std::nullopt, std::nullopt, std::nullopt };
            }
        }

        void Close()
        {
            m_connection.Close();
        }
    };
} // namespace Chess::Net
