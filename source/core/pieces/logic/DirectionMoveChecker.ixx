module;
#include <memory>
#include <vector>
export module Chess.DirectionMoveChecker;
import Chess.Coordinate;
import Chess.ePieceColor;
import Chess.Piece;
import Chess.PieceFinder;
import Chess.PositionChecker;

namespace Chess
{
    export class DirectionMoveChecker
    {
    public:
        static std::vector<Coordinate> FindPossibleMoves(
            const std::shared_ptr<PieceFinder>& finder, Coordinate position, ePieceColor color, std::pair<int, int> direction)
        {
            std::vector<Coordinate> moves;

            while (true)
            {
                position.file = position.file + direction.first;
                position.rank = position.rank + direction.second;

                if (!PositionChecker::IsInChessboard(position))
                {
                    break;
                }

                if (const auto found = finder->TryFind(position))
                {
                    if (found->GetColorAndType().color != color)
                    {
                        moves.emplace_back(position);
                    }

                    break;
                }

                moves.emplace_back(position);
            }

            return moves;
        }
    };
} // namespace Chess
