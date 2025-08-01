export module Chess.Queen;
import Chess.Coordinate;
import Chess.eError;
import Chess.ePieceColor;
import Chess.ePieceType;
import Chess.ErrorConverter;
import Chess.Piece;
import Chess.PieceColorAndType;
import Chess.Sizes;

namespace Chess
{
    export class Queen final : public Piece
    {
    public:
        Queen(ePieceColor color, const Coordinate& coordinate)
            : Piece(PieceColorAndType(color, ePieceType::QUEEN), coordinate)
        {
        }
    };
} // namespace Chess
