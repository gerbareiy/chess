#include "King.h"

#include "../eError.h"
#include "../ErrorConverter.h"
#include "../Sizes.h"

#include <iostream>
#include <math.h>
#include <stdexcept>

Chess::King::King(ePieceColor color)
{
	m_colorAndType = PieceColorAndType(color, ePieceType::KING);

	switch (color)
	{
	case Chess::ePieceColor::BLACK:
		m_position = Coordinate('E', CHESSBOARD_SIZE);
		break;
	case Chess::ePieceColor::WHITE:
		m_position = Coordinate('E', 1);
		break;
	default:
		throw std::out_of_range(ErrorConverter::ToString(Chess::eError::OUT_OF_CHESSBOARD));
	}
}

bool Chess::King::get_CanMakeCasting() const
{
	return m_canMakeCastling;
}

Chess::PieceColorAndType Chess::King::get_ColorAndType() const
{
	return m_colorAndType;
}

bool Chess::King::get_IsCheck() const
{
	return m_isCheck;
}

Chess::Coordinate Chess::King::get_Position() const
{
	return m_position;
}

void Chess::King::DisableCastling()
{
	m_canMakeCastling = false;
}

void Chess::King::Move(Coordinate to)
{
	DisableCastling();

	m_position = to;
}
