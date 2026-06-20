module;
#include <memory>
#include <stdexcept>
#include <vector>
export module Chess.QueenChecker;
import Chess.Coordinate;
import Chess.CoordinateToPieceBuilder;
import Chess.Counts;
import Chess.DirectionMoveChecker;
import Chess.ePieceColor;
import Chess.ePieceType;
import Chess.IMoveChecker;
import Chess.Piece;
import Chess.PieceFinder;
import Chess.Queen;

namespace Chess
{
    export class QueenChecker final : public IMoveChecker
    {
        std::shared_ptr<Queen> m_queen;

    public:
        explicit QueenChecker(const std::shared_ptr<Queen>& queen)
            : m_queen(queen)
        {
        }

        virtual std::vector<Coordinate> GetMoves(const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const override
        {
            if (m_queen == nullptr)
            {
                throw std::logic_error("piece is nullptr");
            }
            auto       pieceMap = CoordinateToPieceBuilder::Build(piecesOnBoard);
            const auto finder   = std::make_shared<PieceFinder>(std::move(pieceMap));

            const auto position = m_queen->GetPosition();
            const auto color    = m_queen->GetColorAndType().color;
            auto       first    = DirectionMoveChecker::FindPossibleMoves(finder, position, color, { -1, -1 });
            auto       second   = DirectionMoveChecker::FindPossibleMoves(finder, position, color, { -1, 1 });
            auto       third    = DirectionMoveChecker::FindPossibleMoves(finder, position, color, { 1, -1 });
            auto       fourth   = DirectionMoveChecker::FindPossibleMoves(finder, position, color, { 1, 1 });
            auto       fifth    = DirectionMoveChecker::FindPossibleMoves(finder, position, color, { -1, 0 });
            auto       sixth    = DirectionMoveChecker::FindPossibleMoves(finder, position, color, { 1, 0 });
            auto       seventh  = DirectionMoveChecker::FindPossibleMoves(finder, position, color, { 0, -1 });
            auto       eighth   = DirectionMoveChecker::FindPossibleMoves(finder, position, color, { 0, 1 });

            std::vector<Coordinate> result;
            result.reserve(QUEEN_WAYS_COUNT);
            result.insert_range(result.end(), std::move(first));
            result.insert_range(result.end(), std::move(second));
            result.insert_range(result.end(), std::move(third));
            result.insert_range(result.end(), std::move(fourth));
            result.insert_range(result.end(), std::move(fifth));
            result.insert_range(result.end(), std::move(sixth));
            result.insert_range(result.end(), std::move(seventh));
            result.insert_range(result.end(), std::move(eighth));
            return result;
        }
    };
} // namespace Chess
