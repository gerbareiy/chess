module;
#include <boost/signals2.hpp>

#include <memory>
#include <vector>
export module Chess.Chessboard;
import Chess.Coordinate;
import Chess.ePieceColor;
import Chess.MoveValidator;
import Chess.Piece;
import Chess.PieceDirector;
import Chess.Player;
import Chess.Promoter;

namespace Chess
{
    export class Chessboard
    {
        std::shared_ptr<PieceDirector>      m_director;
        Coordinate                          m_from = { .file = 0, .rank = 0 };
        std::shared_ptr<MoveValidator>      m_validator;
        std::vector<std::shared_ptr<Piece>> m_piecesOnBoard;
        Coordinate                          m_to = { .file = 0, .rank = 0 };
        std::shared_ptr<Player>             m_player;

        boost::signals2::signal<void()> m_signalChessboardUndated;

    public:
        Chessboard(const std::vector<std::shared_ptr<Piece>>& piecesOnBoard)
            : m_piecesOnBoard(piecesOnBoard)
        {
            m_player    = std::make_shared<Player>(ePieceColor::WHITE);
            m_director  = std::make_shared<PieceDirector>(m_piecesOnBoard, m_player);
            m_validator = std::make_shared<MoveValidator>(m_piecesOnBoard, m_player);
        }

        Coordinate GetFrom() const
        {
            return m_from;
        }

        const std::shared_ptr<MoveValidator>& GetMoveValidator() const
        {
            return m_validator;
        }

        const std::shared_ptr<PieceDirector>& GetPieceDirector() const
        {
            return m_director;
        }

        Coordinate GetTo() const
        {
            return m_to;
        }

        bool TryInitPiece(const Coordinate& from)
        {
            m_from = from;
            m_to   = Coordinate(0, 0);
            m_director->InitCurrentPiece(from);

            if (!m_director->GetCurrentPiece())
            {
                return false;
            }

            m_validator->CalculatePossibleMoves(m_director->GetCurrentPiece());

            if (m_validator->GetPossibleMoves().size() < 1)
            {
                return false;
            }

            m_signalChessboardUndated();

            return true;
        }

        // Before use this method, you need to InitPiece
        bool TryMovePiece(const Coordinate& to, const std::shared_ptr<Promoter>& promoter)
        {
            m_to = to;

            if (!m_validator->IsValidMove(m_director->GetCurrentPiece(), to))
            {
                return false;
            }

            m_validator->ClearPossibleMoves();
            m_validator->ClearPiecesCanMove();
            m_director->MovePiece(to, m_signalChessboardUndated, promoter);
            m_validator->CalculatePiecesCanMove();

            m_signalChessboardUndated();

            return true;
        }

        boost::signals2::connection ConnectChessboardUpdated(const boost::signals2::signal<void()>::slot_type& subscriber)
        {
            return m_signalChessboardUndated.connect(subscriber);
        }
    };
} // namespace Chess
