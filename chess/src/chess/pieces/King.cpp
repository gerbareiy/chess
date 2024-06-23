#include "King.h"

#include "../logic/eError.h"
#include "../logic/ErrorConverter.h"
#include "../logic/Sizes.h"

#include <math.h>
#include <stdexcept>

Chess::King::King(ePieceColor color) : m_colorAndType(PieceColorAndType(color, ePieceType::KING))
{
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

Chess::King::King(ePieceColor color, const std::shared_ptr<PieceSignalDirector>& signalDirector) : King(color)
{
	if (!signalDirector)
	{
		return;
	}

	signalDirector->ConnectMoveWithCheck([this](bool isCheck)
		{
			m_isCheck = isCheck;
		});
}

Chess::King::King(ePieceColor color, Coordinate coordinate)
	: m_colorAndType(PieceColorAndType(color, ePieceType::KING)), m_position(coordinate) { }

Chess::King::King(ePieceColor color, Coordinate coordinate, const std::shared_ptr<PieceSignalDirector>& signalDirector) : King(color, coordinate)
{
	if (!signalDirector)
	{
		return;
	}

	signalDirector->ConnectMoveWithCheck([this](bool isCheck)
		{
			m_isCheck = isCheck;
		});
}

bool Chess::King::get_CanMakeCastling() const
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

void Chess::King::Move(Coordinate to, bool isRealMove)
{
	if(isRealMove)
	{
		DisableCastling();

		if (abs(get_Position().get_File() - to.get_File()) > 1)
		{
			eCastleSide side;

			if (to.get_File() > get_Position().get_File())
			{
				side = eCastleSide::RIGHT;
			}
			else if (to.get_File() < get_Position().get_File())
			{
				side = eCastleSide::LEFT;
			}
			else
			{
				throw std::invalid_argument(ErrorConverter::ToString(eError::OUT_OF_CHESSBOARD));
			}

			m_signalCastling(to, side);
		}
	}

	m_position = to;
}

boost::signals2::connection Chess::King::ConnectCastling(const boost::signals2::signal<void(Coordinate, eCastleSide)>::slot_type& subscriber)
{
	return m_signalCastling.connect(subscriber);
}
