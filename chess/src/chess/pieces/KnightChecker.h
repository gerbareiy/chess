#pragma once

#include "logic/IMoveChecker.h"

#include <memory>
#include <vector>

namespace Chess
{
	class Piece;
	class Knight;
	struct Coordinate;

	class KnightChecker : public IMoveChecker
	{
	private:
		std::vector<std::pair<int, int>> const m_knightMoveDirections =
		{
			{ 2, 1 }, { 2, -1 }, { -2, 1 }, { -2, -1 },
			{ 1, 2 }, { 1, -2 }, { -1, 2 }, { -1, -2 }
		};

	private:
		std::vector<Coordinate> FindPossibleMoves(std::shared_ptr<Knight> const& knight, std::vector<std::shared_ptr<Piece>> const& piecesOnBoard) const;

	public:
		std::vector<Coordinate> GetMoves(std::shared_ptr<Piece> const& piece, std::vector<std::shared_ptr<Piece>> const& piecesOnBoard) const override;
	};
}
