#pragma once

#include "Queen.h"
#include "logic/IMoveChecker.h"

#include <vector>

namespace Chess
{
	struct Coordinate;

	class QueenChecker : public IMoveChecker
	{
	private:
		std::vector<Coordinate> FindPossibleMoves(std::shared_ptr<Queen> const& queen, std::vector<std::shared_ptr<Piece>> const& piecesOnBoard) const;

	public:
		std::vector<Coordinate> GetMoves(std::shared_ptr<Piece> const& piece, std::vector<std::shared_ptr<Piece>> const& piecesOnBoard) const override;
	};
}
