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
		PieceFinder(std::vector<std::shared_ptr<Piece>> const& pieces);

	public:
		std::shared_ptr<Piece> Find(Coordinate const& coordinate) const;
	};
}
