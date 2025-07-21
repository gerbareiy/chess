module;
#include <stdexcept>
export module Chess.Knight;
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
    export class Knight final : public Piece
    {
    public:
        Knight(ePieceColor color, int orderNumber)
        {
            m_colorAndType  = { .color = color, .type = ePieceType::KNIGHT };
            const auto file = orderNumber == 1 ? 'B' : 'G';

            switch (color)
            {
            case ePieceColor::BLACK:
                m_position = { .file = file, .rank = CHESSBOARD_SIZE };
                break;
            case ePieceColor::WHITE:
                m_position = { .file = file, .rank = 1 };
                break;
            default:
                throw std::out_of_range(ErrorConverter::ToString(eError::OUT_OF_CHESSBOARD));
            }
        }

        Knight(ePieceColor color, const Coordinate& coordinate)
            : Piece(PieceColorAndType(color, ePieceType::KNIGHT), coordinate)
        {
        }
    };
} // namespace Chess
