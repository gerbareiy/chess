#pragma once

#include "Coordinate.h"
#include "../pieces/IPiece.h"

#include <memory>
#include <vector>

namespace Chess
{
	class PieceTakeLocator
	{
	public:
		Chess::Coordinate Find(const std::shared_ptr<IPiece>& piece, const std::vector<std::shared_ptr<IPiece>>& piecesOnBoard, Coordinate to) const;
	};
}
