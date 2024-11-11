#include "Queen.h"

#include "logic/PieceColorAndType.h"
#include "../logic/Coordinate.h"
#include "../logic/eError.h"
#include "../logic/ErrorConverter.h"
#include "../logic/Sizes.h"

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

Chess::Queen::Queen(ePieceColor color, Coordinate coordinate)
	: Piece(PieceColorAndType(color, ePieceType::QUEEN), coordinate) { }
