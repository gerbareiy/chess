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
		MoveChecker(const std::shared_ptr<Piece>& piece);

	private:
		std::vector<Coordinate> FindUncheckedMove(const Chess::Coordinate& move, const std::vector<std::shared_ptr<Chess::Piece>>& piecesOnBoard) const;

	public:
		std::vector<Coordinate> GetFilteredMoves(const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const;
	};
}
