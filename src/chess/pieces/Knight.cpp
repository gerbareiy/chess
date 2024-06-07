#include "Knight.h"

#include "ePieceColor.h"
#include "ePieceType.h"
#include "../Sizes.h"
#include "../eError.h"
#include "../ErrorConverter.h"

#include <stdexcept>

Chess::Knight::Knight(ePieceColor color, int orderNumber)
{
	if (orderNumber < 1 || orderNumber > 2)
	{
		throw std::out_of_range(ErrorConverter::ToString(Chess::eError::OUT_OF_COUNT_OF_BISHOP_KNIGHT_ROOK_WITH_1_COLOR));
	}

	m_colorAndType = PieceColorAndType(color, ePieceType::KNIGHT);

	auto file = orderNumber == 1 ? 'B' : 'G';

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

void Chess::Knight::Move(Coordinate to)
{
	m_position = to;
}
