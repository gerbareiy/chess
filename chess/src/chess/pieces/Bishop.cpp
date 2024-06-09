#include "Bishop.h"

#include "ePieceColor.h"
#include "ePieceType.h"
#include "../eError.h"
#include "../ErrorConverter.h"
#include "../Sizes.h"

#include <stdexcept>

Chess::Bishop::Bishop(ePieceColor color, int orderNumber)
{
	if (orderNumber < 1 || orderNumber > 2)
	{
		throw std::out_of_range(ErrorConverter::ToString(Chess::eError::OUT_OF_COUNT_OF_BISHOP_KNIGHT_ROOK_WITH_1_COLOR));
	}

	m_colorAndType = PieceColorAndType(color, ePieceType::BISHOP);

	auto file = orderNumber == 1 ? 'C' : 'F';

	switch (color)
	{
	case Chess::ePieceColor::BLACK:
		m_position = Coordinate(file, CHESSBOARD_SIZE);
		break;
	case Chess::ePieceColor::WHITE:
		m_position = Coordinate(file, 1);
		break;
	default:
		throw std::out_of_range(ErrorConverter::ToString(Chess::eError::OUT_OF_CHESSBOARD));
	}
}

Chess::PieceColorAndType Chess::Bishop::get_ColorAndType() const
{
	return m_colorAndType;
}

Chess::Coordinate Chess::Bishop::get_Position() const
{
	return m_position;
}

void Chess::Bishop::Move(Coordinate to)
{
	m_position = to;
}
