#pragma once

#include "../Coordinate.h"
#include "IPiece.h"
#include "IMoveChecker.h"
#include "Knight.h"

#include <memory>

namespace Chess
{
	class KnightChecker : public IMoveChecker
	{
	private:
		const std::vector<std::pair<int, int>> m_knightMoves =
		{
			{ 2, 1 }, { 2, -1 }, { -2, 1 }, { -2, -1 },
			{ 1, 2 }, { 1, -2 }, { -1, 2 }, { -1, -2 }
		};

	private:
		std::vector<Coordinate> FilterMoves(std::vector<Coordinate> moves, const std::shared_ptr<Knight> piece, const std::vector<std::shared_ptr<IPiece>> piecesOnBoard);
		std::vector<Coordinate> GetAllMoves(const std::shared_ptr<Knight> piece, const std::vector<std::shared_ptr<IPiece>> piecesOnBoard);

	public:
		std::vector<Coordinate> GetPossibleMoves(const std::shared_ptr<IPiece> piece, const std::vector<std::shared_ptr<IPiece>> piecesOnBoard);
	};
}
