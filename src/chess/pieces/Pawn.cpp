#include "Pawn.h"

#include "../Sizes.h"
#include "../eError.h"
#include "../ErrorConverter.h"

#include <stdexcept>

Chess::Pawn::Pawn(ePieceColor color, char file)
{
	if (file < 'A' || file > 'A' + CHESSBOARD_SIZE - 1)
	{
		throw std::out_of_range(ErrorConverter::ToString(Chess::eError::OUT_OF_CHESSBOARD));
	}

	m_colorAndType = PieceColorAndType(color, ePieceType::PAWN);

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
	m_position = to;
}
