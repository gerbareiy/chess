#pragma once

#include "Piece.h"
#include "logic/ePieceColor.h"
#include "logic/PieceColorAndType.h"
#include "../logic/Coordinate.h"

namespace Chess
{
	class Knight : public Piece
	{
	public:
		Knight(ePieceColor pieceColor, int orderNumber);
		Knight(ePieceColor color, Coordinate coordinate);
	};
}
