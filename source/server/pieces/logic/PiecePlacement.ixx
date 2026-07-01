export module Chess.PiecePlacement;
import Chess.Coordinate;
import Chess.PieceColorAndType;

namespace Chess
{
    // Описание одной фигуры на доске: её цвет с типом и позиция.
    export struct PiecePlacement
    {
        PieceColorAndType colorAndType;
        Coordinate        coordinate;

        bool operator==(const PiecePlacement& other) const = default;
    };
} // namespace Chess
