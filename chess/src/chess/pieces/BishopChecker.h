#pragma once

#include "logic/IMoveChecker.h"

#include <memory>
#include <vector>

namespace Chess
{
	class Bishop;
	struct Coordinate;

	class BishopChecker : public IMoveChecker
	{
	private:
		std::vector<Coordinate> FindPossibleMoves(std::shared_ptr<Bishop> const& bishop, std::vector<std::shared_ptr<Piece>> const& piecesOnBoard) const;

	public:
		std::vector<Coordinate> GetMoves(std::shared_ptr<Piece> const& piece, std::vector<std::shared_ptr<Piece>> const& piecesOnBoard) const override;
	};
}
