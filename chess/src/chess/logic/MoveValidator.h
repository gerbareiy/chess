#pragma once

#include "Coordinate.h"
#include "CheckChecker.h"
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
		std::shared_ptr<MoveChecker> m_moveChecker{};
		std::shared_ptr<CheckChecker> m_checkChecker{};
		const std::vector<std::shared_ptr<IPiece>> m_piecesOnBoard;

	public:
		MoveValidator(const std::vector<std::shared_ptr<IPiece>>& piecesOnBoard);

	public:
		std::vector<Coordinate> get_PossibleMoves();

	public:
		void CalculatePossibleMoves(std::shared_ptr<IPiece> piece);
		void ClearPossibleMoves();
		bool IsCoordinateInPossibleMoves(Coordinate coordinate);
		bool IsValidMove(std::shared_ptr<IPiece> piece, Coordinate to);
	};
}
