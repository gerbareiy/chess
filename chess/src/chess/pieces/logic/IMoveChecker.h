#pragma once

#include <memory>
#include <vector>

namespace Chess
{
	class Piece;
	struct Coordinate;

	class IMoveChecker
	{
	public:
		virtual ~IMoveChecker() = default;

	public:
		virtual std::vector<Coordinate> GetMoves(std::shared_ptr<Piece> const& piece, std::vector<std::shared_ptr<Piece>> const& piecesOnBoard) const = 0;
	};
}
