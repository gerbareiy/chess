#pragma once

#include "logic/IMoveChecker.h"

#include <memory>
#include <vector>

namespace Chess
{
	class Piece;
	class PieceFinder;
	class King;

	class KingChecker : public IMoveChecker
	{
	private:
		std::vector<Coordinate> FindCastlingMoves(std::shared_ptr<King> const& king, std::shared_ptr<PieceFinder> const& finder) const;
		std::vector<Coordinate> FindPossibleMoves(std::shared_ptr<King> const& piece, std::vector<std::shared_ptr<Piece>> const& piecesOnBoard) const;

	public:
		std::vector<Coordinate> GetMoves(std::shared_ptr<Piece> const& piece, std::vector<std::shared_ptr<Piece>> const& piecesOnBoard) const override;
	};
}
