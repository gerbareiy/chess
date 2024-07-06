#pragma once

#include "../Piece.h"
#include "../../logic/Coordinate.h"

#include <map>
#include <memory>
#include <vector>

namespace Chess
{
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
