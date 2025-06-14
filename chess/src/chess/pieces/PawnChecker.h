#pragma once

#include "logic/IMoveChecker.h"

#include <memory>
#include <vector>

namespace Chess
{
	class Pawn;
	class Piece;
	class PieceFinder;

	class PawnChecker :	public IMoveChecker
	{
	private:
		std::vector<Coordinate> GetForwardMoves(std::shared_ptr<Pawn> const& pawn, std::vector<std::shared_ptr<Piece>> const& piecesOnBoard, std::shared_ptr<PieceFinder> const& finder) const;
		std::vector<Coordinate> GetDiagonalMoves(std::shared_ptr<Pawn> const& pawn, std::vector<std::shared_ptr<Piece>> const& piecesOnBoard, std::shared_ptr<PieceFinder> const& finder) const;
		void ValidatePawn(std::shared_ptr<Pawn> const& pawn) const;

	public:
		std::vector<Coordinate> GetMoves(std::shared_ptr<Piece> const& piece, std::vector<std::shared_ptr<Piece>> const& piecesOnBoard) const override;
	};
}
