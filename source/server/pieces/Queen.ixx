export module Chess.Queen;
import Chess.Coordinate;
import Chess.ePieceColor;
import Chess.ePieceType;
import Chess.Piece;
import Chess.PieceColorAndType;
import Chess.Sizes;

namespace Chess
{
    export class Queen final : public Piece
    {
    public:
        Queen(ePieceColor color, const Coordinate& coordinate)
            : Piece(color, coordinate)
        {
        }

        virtual PieceColorAndType GetColorAndType() const override
        {
            return { GetColor(), ePieceType::QUEEN };
        }
    };
} // namespace Chess
