module;
#include <stdexcept>
export module Chess.Queen;
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
        Queen(ePieceColor color)
        {
            m_colorAndType = PieceColorAndType(color, ePieceType::QUEEN);

            switch (color)
            {
            case ePieceColor::BLACK:
                m_position = Coordinate('D', CHESSBOARD_SIZE);
                break;
            case ePieceColor::WHITE:
                m_position = Coordinate('D', 1);
                break;
            default:
                throw std::out_of_range(ErrorConverter::ToString(eError::OUT_OF_CHESSBOARD));
            }
        }

        Queen(ePieceColor color, const Coordinate& coordinate)
            : Piece(PieceColorAndType(color, ePieceType::QUEEN), coordinate)
        {
        }
    };
} // namespace Chess
