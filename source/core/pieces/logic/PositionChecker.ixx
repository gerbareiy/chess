export module Chess.Core.PositionChecker;
import Chess.Constants.Sizes;
import Chess.Core.Coordinate;

namespace Chess::Core
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
} // namespace Chess::Core
