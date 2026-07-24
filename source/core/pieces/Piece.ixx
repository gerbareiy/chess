export module Chess.Core.Piece;
import Chess.Core.Coordinate;
import Chess.Core.ePieceColor;
import Chess.Core.PieceColorAndType;

namespace Chess::Core
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
} // namespace Chess::Core
