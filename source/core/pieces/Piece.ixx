export module Chess.Piece;
import Chess.Coordinate;
import Chess.ePieceColor;
import Chess.PieceColorAndType;

namespace Chess
{
    export class Piece
    {
        ePieceColor m_color    = {};
        Coordinate  m_position = {};

    protected:
        ePieceColor GetColor() const
        {
            return m_color;
        }

    public:
        Piece(const ePieceColor& color, const Coordinate& coordinate)
            : m_color(color)
            , m_position(coordinate)
        {
        }

        virtual ~Piece() = default;

        virtual PieceColorAndType GetColorAndType() const = 0;

        virtual void Move(Coordinate to)
        {
            m_position = to;
        }

        Coordinate GetPosition() const
        {
            return m_position;
        }
    };
} // namespace Chess
