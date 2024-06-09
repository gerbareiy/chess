#include "Queen.h"

#include "../eError.h"
#include "../ErrorConverter.h"
#include "../Sizes.h"

#include <iostream>
#include <math.h>
#include <stdexcept>

Chess::Queen::Queen(ePieceColor color)
{
	m_colorAndType = PieceColorAndType(color, ePieceType::QUEEN);

	switch (color)
	{
	case Chess::ePieceColor::BLACK:
		m_position = Coordinate('D', CHESSBOARD_SIZE);
		break;
	case Chess::ePieceColor::WHITE:
		m_position = Coordinate('D', 1);
		break;
	default:
		throw std::out_of_range(ErrorConverter::ToString(Chess::eError::OUT_OF_CHESSBOARD));
	}
}

void Chess::Queen::Move(Coordinate to)
{
	m_position = to;
}
