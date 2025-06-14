#pragma once

#include "logic/IMoveChecker.h"

namespace Chess
{
	class Rook;

	class RookChecker : public IMoveChecker
	{
	private:
		std::vector<Coordinate> FindPossibleMoves(std::shared_ptr<Rook> const& rook, std::vector<std::shared_ptr<Piece>> const& piecesOnBoard) const;

	public:
		std::vector<Coordinate> GetMoves(std::shared_ptr<Piece> const& piece, std::vector<std::shared_ptr<Piece>> const& piecesOnBoard) const override;
	};
}
