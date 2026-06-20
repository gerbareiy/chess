export module Chess.Knight;
import Chess.Coordinate;
import Chess.ePieceColor;
import Chess.ePieceType;
import Chess.Piece;
import Chess.PieceColorAndType;
import Chess.Sizes;

namespace Chess
{
    export class Knight final : public Piece
    {
    public:
        Knight(ePieceColor color, const Coordinate& coordinate)
            : Piece(color, coordinate)
        {
        }

        virtual PieceColorAndType GetColorAndType() const override
        {
            return { GetColor(), ePieceType::KNIGHT };
        }
    };
} // namespace Chess
