#pragma once

#include "IMoveChecker.h"

#include <memory>

namespace Chess
{
	class Piece;

	class MoveChecker
	{
	private:
		std::shared_ptr<IMoveChecker> m_moveCheckerOfPiece;
		std::shared_ptr<Piece> m_piece;

	public:
		MoveChecker(std::shared_ptr<Piece> const& piece);

	private:
		std::vector<Coordinate> FindUncheckedMove(Chess::Coordinate const& move, std::vector<std::shared_ptr<Chess::Piece>> const& piecesOnBoard) const;

	public:
		std::vector<Coordinate> GetFilteredMoves(std::vector<std::shared_ptr<Piece>> const& piecesOnBoard) const;
	};
}
