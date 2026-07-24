export module Chess.PositionChecker;
import Chess.Coordinate;
import Chess.Constants.Sizes;

namespace Chess
{
    export class PositionChecker
    {
    public:
        static bool IsInChessboard(const Coordinate& position)
        {
            return position.file >= 'A' && position.file < 'A' + Constants::Sizes::CHESSBOARD_SIZE && position.rank >= 1
                   && position.rank <= Constants::Sizes::CHESSBOARD_SIZE;
        }
    };
} // namespace Chess
