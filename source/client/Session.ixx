module;
#include "Envelope.pb.h"

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
export module Chess.Client.Session;
import Chess.Chessboard;
import Chess.ChessboardFactory;
import Chess.eGameState;
import Chess.ePieceColor;
import Chess.FixedPromoter;
import Chess.Move;
import Chess.Net.ClientConnection;
import Chess.Proto.Chessboard;
import Chess.Proto.Move;
import Chess.Proto.PieceColorAndType;
import Chess.Proto.Session;

namespace Chess::Client
{
    export enum class eServerEvent
    {
        OpponentMoved,
        Resynced,
        GameOver
    };

    // Клиентская сессия: локальная копия партии + связь с сервером. Ходы валидируются локально
    // и отправляются; входящие сообщения применяют чужой ход, ресинкают доску или завершают партию.
    export class Session
    {
        Chess::Net::ClientConnection       m_connection;
        std::shared_ptr<Chess::Chessboard> m_chessboard;
        Chess::ePieceColor                 m_myColor    = Chess::ePieceColor::NONE;
        bool                               m_gameOver   = false;
        Chess::eGameState                  m_finalState = Chess::eGameState::PLAYING;
        uint32_t                           m_ply        = 0;

        void Rebuild(const chess::proto::Chessboard& board)
        {
            auto       pieces = Chess::Proto::Chessboard::FromProto(board);
            const auto side   = Chess::Proto::PieceColorAndType::FromProto(board.side_to_move());
            m_chessboard      = Chess::ChessboardFactory::Create(std::move(pieces), side);
            m_ply             = board.ply();
        }

        bool ApplyMove(const Chess::Move& move)
        {
            if (!m_chessboard->TrySelectPiece(move.from))
            {
                return false;
            }
            if (!m_chessboard->TryMovePiece(move.to, std::make_shared<Chess::FixedPromoter>(move.promotion)))
            {
                return false;
            }
            ++m_ply;
            return true;
        }

        Session(const std::string& host, unsigned short port)
            : m_connection(Chess::Net::ClientConnection::Connect(host, port))
        {
            chess::proto::Envelope findGame;
            findGame.mutable_find_game();
            m_connection.SendBytes(findGame.SerializeAsString());

            chess::proto::Envelope started;
            started.ParseFromString(m_connection.ReceiveBytes());

            m_myColor = Chess::Proto::PieceColorAndType::FromProto(started.game_started().your_color());
            Rebuild(started.game_started().board());
        }

    public:
        static std::unique_ptr<Session> Connect(const std::string& host, unsigned short port)
        {
            return std::unique_ptr<Session>(new Session(host, port));
        }

        Chess::ePieceColor GetMyColor() const
        {
            return m_myColor;
        }

        const std::shared_ptr<Chess::Chessboard>& GetChessboard() const
        {
            return m_chessboard;
        }

        bool IsMyTurn() const
        {
            return !m_gameOver && m_chessboard->GetSideToMove() == m_myColor;
        }

        bool IsGameOver() const
        {
            return m_gameOver;
        }

        Chess::eGameState GetFinalState() const
        {
            return m_finalState;
        }

        // Локально валидирует и применяет ход, затем отправляет его серверу. false — ход нелегален.
        bool SubmitMove(const Chess::Move& move)
        {
            if (!IsMyTurn() || !ApplyMove(move))
            {
                return false;
            }

            chess::proto::Envelope envelope;
            *envelope.mutable_move() = Chess::Proto::Move::ToProto(move);
            m_connection.SendBytes(envelope.SerializeAsString());
            return true;
        }

        // Блокирующе ждёт следующее сообщение сервера и обрабатывает его.
        eServerEvent ReceiveNext()
        {
            chess::proto::Envelope envelope;
            envelope.ParseFromString(m_connection.ReceiveBytes());

            switch (envelope.payload_case())
            {
            case chess::proto::Envelope::kMove:
                ApplyMove(Chess::Proto::Move::FromProto(envelope.move()));
                return eServerEvent::OpponentMoved;
            case chess::proto::Envelope::kBoardSync:
                // Принудительный ресинк (отклонение хода) — доверяем серверу безусловно.
                Rebuild(envelope.board_sync());
                return eServerEvent::Resynced;
            case chess::proto::Envelope::kBoardCheck:
                // Периодическая сверка: применяем, только если она не устарела относительно нашей
                // локальной партии (иначе откатили бы уже сделанный нами ход).
                if (envelope.board_check().ply() >= m_ply)
                {
                    Rebuild(envelope.board_check());
                }
                return eServerEvent::Resynced;
            case chess::proto::Envelope::kGameOver:
                m_gameOver   = true;
                m_finalState = Chess::Proto::Session::FromProto(envelope.game_over().state());
                Rebuild(envelope.game_over().board());
                return eServerEvent::GameOver;
            default:
                return eServerEvent::Resynced;
            }
        }
    };
} // namespace Chess::Client
