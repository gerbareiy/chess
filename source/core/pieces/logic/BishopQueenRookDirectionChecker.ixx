module;
#include <memory>
#include <vector>
export module Chess.BishopQueenRookDirectionChecker;
import Chess.Coordinate;
import Chess.Piece;
import Chess.PieceFinder;
import Chess.PositionChecker;

namespace Chess
{
    export class BishopQueenRookDirectionChecker
    {
    public:
        static std::vector<Coordinate> FindPossibleMoves(const std::shared_ptr<PieceFinder>& finder,
                                                         const std::shared_ptr<Piece>&       current,
                                                         std::pair<int, int>                 coordinateIncrement)
        {
            std::vector<Coordinate> moves;
            auto                    currentCoordinate = current->GetPosition();

            while (true)
            {
                currentCoordinate.file = currentCoordinate.file + coordinateIncrement.first;
                currentCoordinate.rank = currentCoordinate.rank + coordinateIncrement.second;

                if (!PositionChecker::IsPositionValid(currentCoordinate))
                {
                    break;
                }

                if (const auto found = finder->Find(currentCoordinate))
                {
                    if (found->GetColorAndType().color != current->GetColorAndType().color)
                    {
                        moves.emplace_back(currentCoordinate);
                    }

                    break;
                }

                moves.emplace_back(currentCoordinate);
            }

            return moves;
        }
    };
} // namespace Chess
