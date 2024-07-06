#pragma once

#include "Bishop.h"
#include "logic/IMoveChecker.h"
#include "../logic/Coordinate.h"

#include <memory>
#include <vector>

namespace Chess
{
	class BishopChecker : public IMoveChecker
	{
	private:
		std::vector<Coordinate> FindPossibleMoves(const std::shared_ptr<Bishop>& bishop, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const;

	public:
		std::vector<Coordinate> GetMoves(const std::shared_ptr<Piece>& piece, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const override;
	};
}
