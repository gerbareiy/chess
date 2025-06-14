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
		Piece(PieceColorAndType const& colorAndType, Coordinate const& coordinate);
		virtual ~Piece() = default;

	public:
		PieceColorAndType GetColorAndType() const;
		Coordinate GetPosition() const;

	public:
		virtual void Move(Coordinate to, bool isRealMove = true);
	};
}
