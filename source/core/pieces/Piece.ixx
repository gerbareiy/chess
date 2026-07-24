export module Chess.Piece;
import Chess.Coordinate;
import Chess.ePieceColor;
import Chess.PieceColorAndType;

namespace Chess
{
    export class Piece
    {
    public:
        Piece(const ePieceColor& color, const Coordinate& coordinate)
            : color_(color)
            , position_(coordinate)
        {
        }

        virtual ~Piece() = default;

        virtual PieceColorAndType GetColorAndType() const = 0;

        virtual void Move(Coordinate to)
        {
            position_ = to;
        }

        Coordinate GetPosition() const
        {
            return position_;
        }

    protected:
        ePieceColor GetColor() const
        {
            return color_;
        }

    private:
        ePieceColor color_    = {};
        Coordinate  position_ = {};
    };
} // namespace Chess
