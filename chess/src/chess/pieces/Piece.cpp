#include "Piece.h"

Chess::Piece::Piece() : m_colorAndType(ePieceColor::NONE, ePieceType::NONE) { }

Chess::Piece::Piece(PieceColorAndType colorAndType, Coordinate coordinate)
    : m_colorAndType(colorAndType), m_position(coordinate) { }

Chess::PieceColorAndType Chess::Piece::get_ColorAndType() const
{
    return m_colorAndType;
}

Chess::Coordinate Chess::Piece::get_Position() const
{
    return m_position;
}

void Chess::Piece::Move(Coordinate to, bool isRealMove)
{
    m_position = to;
}
