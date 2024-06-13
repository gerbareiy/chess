#pragma once
#include "IMoveChecker.h"

#include "Rook.h"

namespace Chess
{
	class RookChecker : public IMoveChecker
	{
	private:
		std::vector<Coordinate> FindPossibleMoves(const std::shared_ptr<Rook>& rook, const std::vector<std::shared_ptr<IPiece>>& piecesOnBoard);

	public:
		std::vector<Coordinate> GetPossibleMoves(const std::shared_ptr<IPiece>& piece, const std::vector<std::shared_ptr<IPiece>>& piecesOnBoard);
	};
}
