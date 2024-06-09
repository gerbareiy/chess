#include "Pawn.h"

#include "../eError.h"
#include "../ErrorConverter.h"
#include "../Sizes.h"

#include <stdexcept>

Chess::Pawn::Pawn(ePieceColor color, char file) : m_colorAndType(color, ePieceType::PAWN)
{
	if (file < 'A' || file > 'A' + CHESSBOARD_SIZE - 1)
	{
		throw std::out_of_range(ErrorConverter::ToString(Chess::eError::OUT_OF_CHESSBOARD));
	}

	switch (color)
	{
	case Chess::ePieceColor::BLACK:
		m_position = Coordinate(file, CHESSBOARD_SIZE - 1);
		break;
	case Chess::ePieceColor::WHITE:
		m_position = Coordinate(file, 2);
		break;
	default:
		throw std::out_of_range(ErrorConverter::ToString(Chess::eError::OUT_OF_CHESSBOARD));
	}
}

void Chess::Pawn::Move(Coordinate to)
{
	m_isNotMoved = false;

	if (abs(to.get_Rank() - m_position.get_Rank()) == 2)
	{
		m_canEnPassant = true;
	}
	else
	{
		m_canEnPassant = false;
	}

	m_position = to;
}
