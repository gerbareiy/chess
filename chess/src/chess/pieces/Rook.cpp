#include "Rook.h"

#include "logic/ePieceColor.h"
#include "logic/ePieceType.h"
#include "../logic/Sizes.h"
#include "../logic/eError.h"
#include "../logic/ErrorConverter.h"

#include <stdexcept>

Chess::Rook::Rook(ePieceColor color, int orderNumber) : m_canMakeCastling(true)
{
	if (orderNumber < 1 || orderNumber > 2)
	{
		throw std::out_of_range(ErrorConverter::ToString(Chess::eError::OUT_OF_COUNT_OF_BISHOP_KNIGHT_ROOK_WITH_ONE_COLOR));
	}

	m_colorAndType = PieceColorAndType(color, ePieceType::ROOK);

	auto file = orderNumber == 1 ? 'A' : 'A' + CHESSBOARD_SIZE - 1;

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

Chess::Rook::Rook(ePieceColor pieceColor, int orderNumber, std::shared_ptr<IKing> king) : Rook(pieceColor, orderNumber)
{
	if (!king)
	{
		return;
	}

	m_castlingConnection = king->ConnectCastling(std::bind(&Rook::OnCastling, this, std::placeholders::_1, std::placeholders::_2));
}

bool Chess::Rook::get_CanMakeCastling() const
{
	return m_canMakeCastling;
}

Chess::PieceColorAndType Chess::Rook::get_ColorAndType() const
{
	return m_colorAndType;
}

Chess::Coordinate Chess::Rook::get_Position() const
{
	return m_position;
}

void Chess::Rook::DisableCastling()
{
	m_canMakeCastling = false;
}

void Chess::Rook::OnCastling(Coordinate to, eCastleSide side)
{
	if (get_CanMakeCastling())
	{
		if (get_Position().get_File() == 'A' && side == eCastleSide::LEFT)
		{
			Move(Coordinate(to.get_File() + 1, to.get_Rank()));
		}
		else if (get_Position().get_File() == 'A' + CHESSBOARD_SIZE - 1 && side == eCastleSide::RIGHT)
		{
			Move(Coordinate(to.get_File() - 1, to.get_Rank()));
		}
	}

	m_castlingConnection.disconnect();
}

void Chess::Rook::Move(Coordinate to)
{
	DisableCastling();

	m_position = to;
}
