module;
#include <memory>
#include <vector>
export module Chess.BishopChecker;
import Chess.Bishop;
import Chess.Coordinate;
import Chess.CoordinateToPieceFactory;
import Chess.Counts;
import Chess.DirectionMoveChecker;
import Chess.ePieceType;
import Chess.IMoveChecker;
import Chess.Piece;
import Chess.PieceFinder;
import Chess.Utils.Exceptions;

namespace Chess
{
    export class BishopChecker final : public IMoveChecker
    {
        std::shared_ptr<Bishop> m_bishop;

    public:
        explicit BishopChecker(const std::shared_ptr<Bishop>& bishop)
            : m_bishop(bishop)
        {
        }

        virtual std::vector<Coordinate> GetMoves(const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const override
        {
            if (m_bishop == nullptr)
            {
                throw Utils::PieceIsNullptrException();
            }

            auto       pieceMap = CoordinateToPieceFactory::Create(piecesOnBoard);
            const auto finder   = std::make_shared<PieceFinder>(std::move(pieceMap));

            auto first  = DirectionMoveChecker::FindPossibleMoves(finder, m_bishop->GetPosition(), m_bishop->GetColorAndType().color, { -1, -1 });
            auto second = DirectionMoveChecker::FindPossibleMoves(finder, m_bishop->GetPosition(), m_bishop->GetColorAndType().color, { -1, 1 });
            auto third  = DirectionMoveChecker::FindPossibleMoves(finder, m_bishop->GetPosition(), m_bishop->GetColorAndType().color, { 1, -1 });
            auto fourth = DirectionMoveChecker::FindPossibleMoves(finder, m_bishop->GetPosition(), m_bishop->GetColorAndType().color, { 1, 1 });

            std::vector<Coordinate> result;
            result.reserve(BISHOP_WAYS_COUNT);
            result.insert_range(result.end(), std::move(first));
            result.insert_range(result.end(), std::move(second));
            result.insert_range(result.end(), std::move(third));
            result.insert_range(result.end(), std::move(fourth));
            return result;
        }
    };
} // namespace Chess
