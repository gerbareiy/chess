#pragma once

#include "Piece.h"
#include "logic/ePieceColor.h"
#include "logic/PieceColorAndType.h"
#include "../logic/Coordinate.h"

namespace Chess
{
	class Bishop : public Piece
	{
	public:
		Bishop(ePieceColor color, int orderNumber);
		Bishop(ePieceColor color, Coordinate coordinate);
	};
}
