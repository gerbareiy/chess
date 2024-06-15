#pragma once

#include "Bishop.h"
#include "logic/IMoveChecker.h"
#include "../logic/Coordinate.h"

#include <memory>
#include <vector>

namespace Chess
{
	class BishopChecker : public IMoveChecker
	{
	private:
		std::vector<Coordinate> FindPossibleMoves(const std::shared_ptr<Bishop>& bishop, const std::vector<std::shared_ptr<IPiece>>& piecesOnBoard);

	public:
		std::vector<Coordinate> GetPossibleMoves(const std::shared_ptr<IPiece>& piece, const std::vector<std::shared_ptr<IPiece>>& piecesOnBoard);
	};
}
