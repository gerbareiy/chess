#pragma once

#include "IMoveChecker.h"
#include "../IPiece.h"

#include <memory>

namespace Chess
{
	class MoveChecker
	{
	private:
		std::shared_ptr<IMoveChecker> m_moveCheckerOfPiece;
		std::shared_ptr<IPiece> m_piece;

	public:
		MoveChecker(const std::shared_ptr<IPiece>& piece);

	private:
		std::vector<Coordinate> EmulateCheckIfMove(const Chess::Coordinate& move, const std::vector<std::shared_ptr<Chess::IPiece>>& piecesOnBoard);

	public:
		std::vector<Coordinate> GetPossibleMoves(const std::vector<std::shared_ptr<IPiece>>& piecesOnBoard);
	};
}
