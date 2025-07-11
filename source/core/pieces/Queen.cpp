#include "Queen.h"

#include "core/logic/Coordinate.h"
#include "core/logic/ErrorConverter.h"
#include "core/logic/Sizes.h"
#include "core/logic/eError.h"
#include "logic/PieceColorAndType.h"

#include <stdexcept>

Chess::Queen::Queen(ePieceColor color)
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

Chess::Queen::Queen(ePieceColor color, const Coordinate& coordinate)
    : Piece(PieceColorAndType(color, ePieceType::QUEEN), coordinate)
{
}
