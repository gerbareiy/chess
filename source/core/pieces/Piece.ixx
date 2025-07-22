module;
#include <expected>
#include <string>
export module Chess.Piece;
import Chess.Coordinate;
import Chess.PieceColorAndType;

namespace Chess
{
    export class Piece
    {
    protected:
        PieceColorAndType m_colorAndType = {};
        Coordinate        m_position     = {};

    public:
        Piece() = default;
        Piece(const PieceColorAndType& colorAndType, const Coordinate& coordinate)
            : m_colorAndType(colorAndType)
            , m_position(coordinate)
        {
        }

        virtual ~Piece() = default;

        PieceColorAndType GetColorAndType() const
        {
            return m_colorAndType;
        }

        Coordinate GetPosition() const
        {
            return m_position;
        }

        virtual std::expected<void, std::string> Move(Coordinate to, bool isRealMove = true)
        {
            m_position = to;
            return {};
        }
    };
} // namespace Chess
