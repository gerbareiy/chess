module;
#include "core/logic/Coordinate.h"

#include <memory>
#include <vector>
export module Chess.MoveChecker;
import Chess.CheckChecker;
import Chess.IMoveChecker;
import Chess.MoveCheckerFactory;
import Chess.Piece;
import Chess.PieceFinder;

namespace Chess
{
    export class MoveChecker
    {
        std::shared_ptr<IMoveChecker> m_moveCheckerOfPiece;
        std::shared_ptr<Piece>        m_piece;

        std::vector<Coordinate> FindUncheckedMove(const Coordinate& move, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const
        {
            std::vector<Coordinate> filteredMoves;
            const auto              finder        = std::make_shared<PieceFinder>(piecesOnBoard);
            const auto              capturedPiece = finder->Find(move);

            std::vector<std::shared_ptr<Piece>> tempPiecesOnBoard = piecesOnBoard;

            if (capturedPiece)
            {
                std::erase(tempPiecesOnBoard, capturedPiece);
            }

            m_piece->Move(move, false);

            if (!CheckChecker::IsCheck(m_piece->GetColorAndType().color, tempPiecesOnBoard))
            {
                filteredMoves.emplace_back(move);
            }

            return filteredMoves;
        }

    public:
        explicit MoveChecker(const std::shared_ptr<Piece>& piece)
            : m_moveCheckerOfPiece(MoveCheckerFactory::Create(piece))
            , m_piece(piece)
        {
        }

        std::vector<Coordinate> GetFilteredMoves(const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const
        {
            std::vector<Coordinate> filteredMoves;
            const auto              notFilteredMoves = m_moveCheckerOfPiece->GetMoves(m_piece, piecesOnBoard);
            const auto              pieceCoordinate  = m_piece->GetPosition();

            for (const auto& move : notFilteredMoves)
            {
                auto partOfFilteredMoves = FindUncheckedMove(move, piecesOnBoard);
                filteredMoves.insert(filteredMoves.end(), partOfFilteredMoves.begin(), partOfFilteredMoves.end());
            }

            m_piece->Move(pieceCoordinate, false);

            return filteredMoves;
        }
    };
} // namespace Chess
