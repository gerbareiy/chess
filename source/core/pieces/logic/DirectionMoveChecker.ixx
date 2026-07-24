module;
#include <memory>
#include <vector>
export module Chess.Core.DirectionMoveChecker;
import Chess.Core.Coordinate;
import Chess.Core.ePieceColor;
import Chess.Core.Piece;
import Chess.Core.PieceFinder;
import Chess.Core.PositionChecker;

namespace Chess::Core
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
} // namespace Chess::Core
