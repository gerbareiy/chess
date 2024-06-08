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

void Chess::King::Move(Coordinate to)
{
	m_position = to;
}
