export module Chess.Bishop;
import Chess.Coordinate;
import Chess.ePieceColor;
import Chess.ePieceType;
import Chess.Piece;
import Chess.PieceColorAndType;
import Chess.Sizes;

namespace Chess
{
    export class Bishop final : public Piece
    {
    public:
        Bishop(ePieceColor color, const Coordinate& coordinate)
            : Piece(PieceColorAndType(color, ePieceType::BISHOP), coordinate)
        {
        }
    };
} // namespace Chess
