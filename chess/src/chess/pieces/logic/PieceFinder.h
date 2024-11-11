#pragma once

#include <map>
#include <memory>
#include <vector>

namespace Chess
{
	class Piece;
	struct Coordinate;

	class PieceFinder
	{
	private:
		std::map<Coordinate, std::shared_ptr<Piece>> m_pieceMap;

	public:
		PieceFinder(const std::vector<std::shared_ptr<Piece>>& pieces);

	public:
		std::shared_ptr<Piece> Find(Coordinate coordinate) const;
	};
}
