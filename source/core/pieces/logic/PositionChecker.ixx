export module Chess.PositionChecker;
import Chess.Coordinate;
import Chess.Sizes;

namespace Chess
{
    export class PositionChecker
    {
    public:
        static bool IsPositionValid(const Coordinate& position)
        {
            return position.file >= 'A' && position.file < 'A' + CHESSBOARD_SIZE && position.rank >= 1 && position.rank <= CHESSBOARD_SIZE;
        }
    };
} // namespace Chess
