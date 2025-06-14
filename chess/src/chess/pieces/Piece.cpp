#include "Piece.h"

#include "logic/ePieceColor.h"
#include "logic/ePieceType.h"

Chess::Piece::Piece() : m_colorAndType(ePieceColor::NONE, ePieceType::NONE) { }

Chess::Piece::Piece(PieceColorAndType const& colorAndType, Coordinate const& coordinate)
    : m_colorAndType(colorAndType), m_position(coordinate) { }

Chess::PieceColorAndType Chess::Piece::GetColorAndType() const
{
    return m_colorAndType;
}

Chess::Coordinate Chess::Piece::GetPosition() const
{
    return m_position;
}

void Chess::Piece::Move(Coordinate to, bool isRealMove)
{
    m_position = to;
}
