#pragma once

#include "../IPiece.h"
#include "../../logic/Coordinate.h"

#include <map>
#include <memory>
#include <vector>

namespace Chess
{
	class PieceFinder
	{
	private:
		std::map<Coordinate, std::shared_ptr<IPiece>> m_pieceMap;

	public:
		PieceFinder(const std::vector<std::shared_ptr<IPiece>>& pieces);

	public:
		std::shared_ptr<IPiece> Find(Coordinate coordinate);
	};
}
