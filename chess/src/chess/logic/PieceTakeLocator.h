#pragma once

#include "Coordinate.h"
#include "../pieces/Piece.h"

#include <memory>
#include <vector>

namespace Chess
{
	class PieceTakeLocator
	{
	public:
		Chess::Coordinate Find(const std::shared_ptr<Piece>& piece, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard, Coordinate to) const;
	};
}
