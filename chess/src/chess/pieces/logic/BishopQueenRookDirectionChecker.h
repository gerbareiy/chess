#pragma once

#include <memory>
#include <vector>

namespace Chess
{
	class Piece;
	class PieceFinder;
	struct Coordinate;

	class BishopQueenRookDirectionChecker
	{
	public:
		static std::vector<Coordinate> FindPossibleMoves(const std::shared_ptr<PieceFinder>& finder, const std::shared_ptr<Piece>& current, std::pair <int, int> coordinateIncrement);
	};
}
