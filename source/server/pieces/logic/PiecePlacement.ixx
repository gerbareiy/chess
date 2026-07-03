export module Chess.PiecePlacement;
import Chess.Coordinate;
import Chess.PieceColorAndType;

namespace Chess
{
    export struct PiecePlacement
    {
        PieceColorAndType colorAndType;
        Coordinate        coordinate;

        bool operator==(const PiecePlacement& other) const = default;
    };
} // namespace Chess
