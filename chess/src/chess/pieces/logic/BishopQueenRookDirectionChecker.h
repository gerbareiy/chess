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
		static std::vector<Coordinate> FindPossibleMoves(std::shared_ptr<PieceFinder> const& finder, std::shared_ptr<Piece> const& current, std::pair <int, int> coordinateIncrement);
	};
}
