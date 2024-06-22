#include "Knight.h"

#include "logic/ePieceColor.h"
#include "logic/ePieceType.h"
#include "../logic/eError.h"
#include "../logic/ErrorConverter.h"
#include "../logic/Sizes.h"

#include <stdexcept>

Chess::Knight::Knight(ePieceColor color, int orderNumber) : m_colorAndType(PieceColorAndType(color, ePieceType::KNIGHT))
{
	if (orderNumber < 1 || orderNumber > 2)
	{
		throw std::out_of_range(ErrorConverter::ToString(Chess::eError::OUT_OF_COUNT_OF_BISHOP_KNIGHT_ROOK_WITH_ONE_COLOR));
	}

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

Chess::Knight::Knight(ePieceColor color, Coordinate coordinate)
	: m_colorAndType(PieceColorAndType(color, ePieceType::KNIGHT)), m_position(coordinate) { }

Chess::PieceColorAndType Chess::Knight::get_ColorAndType() const
{
	return m_colorAndType;
}

Chess::Coordinate Chess::Knight::get_Position() const
{
	return m_position;
}

void Chess::Knight::Move(Coordinate to)
{
	m_position = to;
}
