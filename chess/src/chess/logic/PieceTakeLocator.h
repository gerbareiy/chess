#pragma once

#include <memory>
#include <vector>

namespace Chess
{
	class Piece;
	struct Coordinate;

	class PieceTakeLocator
	{
	public:
		static Chess::Coordinate Find(std::shared_ptr<Piece> const& piece, std::vector<std::shared_ptr<Piece>> const& piecesOnBoard, Coordinate to);
	};
}
