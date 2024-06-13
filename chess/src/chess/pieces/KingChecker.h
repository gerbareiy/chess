#pragma once

#include "IMoveChecker.h"
#include "IPiece.h"
#include "King.h"

#include <memory>
#include <vector>

namespace Chess
{
	class KingChecker : public IMoveChecker
	{
	private:
		std::vector<Coordinate> FilterMoves(std::vector<Coordinate> moves, const std::shared_ptr<King>& piece, const std::vector<std::shared_ptr<IPiece>>& piecesOnBoard);
		std::vector<Coordinate> FindPossibleMoves(const std::shared_ptr<King>& piece);

	public:
		std::vector<Coordinate> GetPossibleMoves(const std::shared_ptr<IPiece>& piece, const std::vector<std::shared_ptr<IPiece>>& piecesOnBoard);
	};
}
