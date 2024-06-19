#include "Pawn.h"

#include "../logic/eError.h"
#include "../logic/ErrorConverter.h"
#include "../logic/Sizes.h"

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

Chess::Pawn::Pawn(ePieceColor color, char file, std::shared_ptr<PieceSignalDirector> signalDirector) : Pawn(color, file)
{
	if (!signalDirector)
	{
		return;
	}

	signalDirector->ConnectMove([this]()
		{
			if (!m_isOnPawnFirstMove)
			{
				LostEnPassant();
			}

			m_isOnPawnFirstMove = false;
		});
}

bool Chess::Pawn::get_CanEnPassant() const
{
	return m_canEnPassant;
}

Chess::PieceColorAndType Chess::Pawn::get_ColorAndType() const
{
	return m_colorAndType;
}

bool Chess::Pawn::get_IsNotMoved() const
{
	return m_isNotMoved;
}

Chess::Coordinate Chess::Pawn::get_Position() const
{
	return m_position;
}

void Chess::Pawn::LostEnPassant()
{
	m_canEnPassant = false;
}

void Chess::Pawn::Move(Coordinate to)
{
	if (abs(to.get_Rank() - get_Position().get_Rank()) == 2)
	{
		m_canEnPassant = m_isNotMoved;
	}
	else
	{
		LostEnPassant();
	}

	m_isOnPawnFirstMove = m_isNotMoved;
	m_isNotMoved = false;
	m_position = to;
}
