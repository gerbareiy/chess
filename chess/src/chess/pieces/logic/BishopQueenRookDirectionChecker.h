#pragma once

#include "PieceFinder.h"
#include "../Piece.h"
#include "../../logic/Coordinate.h"

#include <memory>
#include <vector>

namespace Chess
{
	class BishopQueenRookDirectionChecker
	{
	public:
		static std::vector<Coordinate> FindPossibleMoves(const std::shared_ptr<PieceFinder>& finder, const std::shared_ptr<Piece>& current, std::pair <int, int> coordinateIncrement);
	};
}
