module;
#include <stdexcept>
export module Chess.Bishop;
import Chess.Piece;
import Chess.eError;
import Chess.ePieceColor;
import Chess.ePieceType;
import Chess.ErrorConverter;
import Chess.PieceColorAndType;
import Chess.Sizes;

namespace Chess
{
    export class Bishop final : public Piece
    {
    public:
        Bishop(ePieceColor color, int orderNumber)
        {

            if (orderNumber < 1 || orderNumber > 2)
            {
                throw std::out_of_range(ErrorConverter::ToString(eError::OUT_OF_COUNT_OF_BISHOP_KNIGHT_ROOK_WITH_ONE_COLOR));
            }

            m_colorAndType = PieceColorAndType(color, ePieceType::BISHOP);

            const auto file = orderNumber == 1 ? 'C' : 'F';

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

        Bishop(ePieceColor color, const Coordinate& coordinate)
            : Piece(PieceColorAndType(color, ePieceType::BISHOP), coordinate)
        {
        }
    };
} // namespace Chess
