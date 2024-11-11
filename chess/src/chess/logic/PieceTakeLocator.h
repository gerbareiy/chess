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
		static Chess::Coordinate Find(const std::shared_ptr<Piece>& piece, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard, Coordinate to);
	};
}
