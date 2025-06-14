#pragma once

#include "Piece.h"
#include "logic/ePieceColor.h"

namespace Chess
{
	struct Coordinate;

	class Queen : public Piece
	{
	public:
		Queen(ePieceColor color);
		Queen(ePieceColor color, Coordinate const& coordinate);
	};
}
