#pragma once

#include "logic/PieceColorAndType.h"
#include "../logic/Coordinate.h"

namespace Chess
{
	class Piece
	{
	protected:
		PieceColorAndType m_colorAndType;
		Coordinate m_position;

	public:
		Piece();
		Piece(PieceColorAndType colorAndType, Coordinate coordinate);
		virtual ~Piece() = default;

	public:
		PieceColorAndType get_ColorAndType() const;
		Coordinate get_Position() const;

	public:
		virtual void Move(Coordinate to, bool isRealMove = true);
	};
}
