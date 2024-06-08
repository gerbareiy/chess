#pragma once

#include "IMoveChecker.h"
#include "IPiece.h"
#include "Pawn.h"

#include <memory>
#include <map>
#include <vector>

namespace Chess
{
	class PawnChecker :	public IMoveChecker
	{
	private:
		std::map<Coordinate, std::shared_ptr<IPiece>> CreatePieceMap(const std::vector<std::shared_ptr<IPiece>>& piecesOnBoard);
		bool IsPositionValid(const Coordinate& position);
		std::vector<Coordinate> FindAllMoves(const std::shared_ptr<Pawn>& pawn, const std::vector<std::shared_ptr<IPiece>>& piecesOnBoard);
		std::vector<Coordinate> GetDiagonalMoves(const std::shared_ptr<Pawn>& pawn, const std::vector<std::shared_ptr<IPiece>>& piecesOnBoard);
		void ValidatePawn(const std::shared_ptr<Pawn>& pawn);

	public:
		std::vector<Coordinate> GetPossibleMoves(const std::shared_ptr<IPiece>& piece, const std::vector<std::shared_ptr<IPiece>>& piecesOnBoard);
	};
}
