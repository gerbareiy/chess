#pragma once

#include "Piece.h"
#include "logic/ePieceColor.h"

namespace Chess
{
	struct Coordinate;
	struct PieceColorAndType;

	class Bishop : public Piece
	{
	public:
		Bishop(ePieceColor color, int orderNumber);
		Bishop(ePieceColor color, Coordinate coordinate);
	};
}
