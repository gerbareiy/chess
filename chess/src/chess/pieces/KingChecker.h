#pragma once

#include "Piece.h"
#include "King.h"
#include "logic/IMoveChecker.h"
#include "logic/PieceFinder.h"

#include <memory>
#include <vector>

namespace Chess
{
	class KingChecker : public IMoveChecker
	{
	private:
		std::vector<Coordinate> FindCastlingMoves(const std::shared_ptr<King>& king, const std::shared_ptr<PieceFinder>& finder) const;
		std::vector<Coordinate> FindPossibleMoves(const std::shared_ptr<King>& piece, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const;

	public:
		std::vector<Coordinate> GetMoves(const std::shared_ptr<Piece>& piece, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const override;
	};
}
