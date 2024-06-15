#pragma once

#include "IMoveChecker.h"

#include <memory>

namespace Chess
{
	class MoveChecker : public IMoveChecker
	{
	public:
		std::vector<Coordinate> GetPossibleMoves(const std::shared_ptr<IPiece>& piece, const std::vector<std::shared_ptr<IPiece>>& piecesOnBoard);
	};
}
