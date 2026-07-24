export module Chess.Core.PiecePlacement;
import Chess.Core.Coordinate;
import Chess.Core.PieceColorAndType;

namespace Chess::Core
{
    export struct PiecePlacement
    {
        PieceColorAndType colorAndType;
        Coordinate        coordinate;

        bool operator==(const PiecePlacement& other) const = default;
    };
} // namespace Chess::Core
