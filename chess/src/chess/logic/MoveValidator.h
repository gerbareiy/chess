#pragma once

#include "Coordinate.h"
#include "../pieces/IPiece.h"
#include "../pieces/logic/MoveChecker.h"

#include <memory>
#include <vector>

namespace Chess
{
	class MoveValidator
	{
	private:
		std::vector<Coordinate> m_possibleMoves{};
		std::shared_ptr<MoveChecker> m_checker{};

	public:
		MoveValidator();

	public:
		std::vector<Coordinate> get_PossibleMoves();

	public:
		void CalculatePossibleMoves(std::shared_ptr<IPiece> piece, std::vector<std::shared_ptr<IPiece>> piecesOnBoard);
		void ClearPossibleMoves();
		bool IsCoordinateInPossibleMoves(Coordinate coordinate);
		bool IsValidMove(std::shared_ptr<IPiece> piece, Coordinate to);
	};
}
