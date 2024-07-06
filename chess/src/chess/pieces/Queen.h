#pragma once

#include "Piece.h"
#include "logic/ePieceColor.h"
#include "logic/PieceColorAndType.h"
#include "../logic/Coordinate.h"

namespace Chess
{
	class Queen : public Piece
	{
	public:
		Queen(ePieceColor color);
		Queen(ePieceColor color, Coordinate coordinate);
	};
}
