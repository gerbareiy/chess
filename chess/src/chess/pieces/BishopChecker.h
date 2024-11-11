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
		std::vector<Coordinate> FindPossibleMoves(const std::shared_ptr<Bishop>& bishop, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const;

	public:
		std::vector<Coordinate> GetMoves(const std::shared_ptr<Piece>& piece, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const override;
	};
}
