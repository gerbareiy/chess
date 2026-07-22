module;
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
import Chess.Net.GameSession;

namespace Chess::Client
{
    export enum class eServerEvent
    {
        OpponentMoved,
        Resynced,
        GameOver
    };

    export class Session
    {
        Net::GameSession            m_session;
        std::shared_ptr<Chessboard> m_chessboard;
        ePieceColor                 m_myColor    = ePieceColor::NONE;
        eGameState                  m_finalState = eGameState::PLAYING;
        bool                        m_gameOver   = false;
        uint32_t                    m_ply        = 0;

        void Rebuild(Net::BoardSnapshot board)
        {
            m_ply        = board.ply;
            m_chessboard = ChessboardFactory::Create(std::move(board.pieces), board.sideToMove);
        }

        bool ApplyMove(const Move& move)
        {
            if (!m_chessboard->TrySelectPiece(move.from))
            {
                return false;
            }
            if (!m_chessboard->TryMovePiece(move.to, std::make_shared<FixedPromoter>(move.promotion)))
            {
                return false;
            }
            ++m_ply;
            return true;
        }

        explicit Session(Net::GameSession session)
            : m_session(std::move(session))
        {
            m_myColor = m_session.GetMyColor();
            Rebuild(m_session.GetInitialBoard());
        }

    public:
        static std::unique_ptr<Session> Connect(const std::string& host, unsigned short port)
        {
            return std::unique_ptr<Session>(new Session(Net::GameSession::Connect(host, port)));
        }

        ePieceColor GetMyColor() const
        {
            return m_myColor;
        }

        const std::shared_ptr<Chessboard>& GetChessboard() const
        {
            return m_chessboard;
        }

        bool IsMyTurn() const
        {
            return !IsGameOver() && m_chessboard->GetSideToMove() == m_myColor;
        }

        bool IsGameOver() const
        {
            return m_gameOver;
        }

        eGameState GetFinalState() const
        {
            return m_finalState;
        }

        bool SubmitMove(const Move& move)
        {
            if (!IsMyTurn() || !ApplyMove(move))
            {
                return false;
            }

            m_session.SendMove(move);
            return true;
        }

        eServerEvent ReceiveNext()
        {
            auto message = m_session.ReceiveNext();

            switch (message.event)
            {
            case Net::eSessionEvent::OpponentMoved:
                ApplyMove(*message.move);
                return eServerEvent::OpponentMoved;
            case Net::eSessionEvent::BoardSynced:
                Rebuild(std::move(*message.board));
                return eServerEvent::Resynced;
            case Net::eSessionEvent::BoardChecked:
                if (message.board->ply >= m_ply)
                {
                    Rebuild(std::move(*message.board));
                }
                return eServerEvent::Resynced;
            case Net::eSessionEvent::GameOver:
                m_gameOver   = true;
                m_finalState = *message.finalState;
                Rebuild(std::move(*message.board));
                return eServerEvent::GameOver;
            default:
                return eServerEvent::Resynced;
            }
        }
    };
} // namespace Chess::Client
