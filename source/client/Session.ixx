module;
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
export module Chess.Client.Session;
import Chess.Chessboard;
import Chess.ChessboardFactory;
import Chess.Client.eServerEvent;
import Chess.eGameState;
import Chess.ePieceColor;
import Chess.FixedPromoter;
import Chess.Move;
import Chess.Net.BoardSnapshot;
import Chess.Net.eSessionEvent;
import Chess.Net.GameSession;

namespace Chess::Client
{
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

        bool TryApplyMove(const Move& move)
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
        }

        void Init()
        {
            m_myColor = m_session.GetMyColor();
            Rebuild(m_session.GetInitialBoard());
        }

    public:
        static Session Connect(const std::string& host, unsigned short port)
        {
            auto session = Session(Net::GameSession::Connect(host, port));
            session.Init();
            return session;
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

        bool TrySubmitMove(const Move& move)
        {
            if (IsMyTurn() && TryApplyMove(move))
            {
                m_session.SendMove(move);
                return true;
            }
            return false;
        }

        eServerEvent ReceiveNext()
        {
            auto [event, move, board, finalState] = m_session.ReceiveNext();

            switch (event)
            {
            case Net::eSessionEvent::OPPONENT_MOVED:
                TryApplyMove(move.value());
                return eServerEvent::OPPONENT_MOVED;
            case Net::eSessionEvent::BOARD_SYNCED:
                Rebuild(std::move(board).value());
                return eServerEvent::RESYNCED;
            case Net::eSessionEvent::BOARD_CHECKED:
                if (board.value().ply >= m_ply)
                {
                    Rebuild(std::move(board).value());
                }
                return eServerEvent::RESYNCED;
            case Net::eSessionEvent::GAME_OVER:
                m_gameOver   = true;
                m_finalState = finalState.value();
                Rebuild(std::move(board).value());
                return eServerEvent::GAME_OVER;
            default:
                return eServerEvent::RESYNCED;
            }
        }
    };
} // namespace Chess::Client
