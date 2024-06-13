#pragma once

#include "Coordinate.h"
#include "pieces/IPiece.h"
#include "pieces/MoveChecker.h"
#include "pieces/PieceColorAndType.h"

#include <memory>
#include <vector>

namespace Chess
{
	class Chessboard
	{
	private:
		std::shared_ptr<MoveChecker> m_checker{};
		std::vector<std::shared_ptr<IPiece>> m_eatenPieces{};
		std::shared_ptr<IPiece> m_carrentPiece{};
		std::vector<std::shared_ptr<IPiece>> m_piecesOnBoard{};
		std::vector<Coordinate> m_possibleMoves{};

	public:
		Chessboard();

	private:
		std::shared_ptr<Chess::IPiece> GetPiece(Coordinate from) const;
		bool IsValidMove(Coordinate to);

	public:
		PieceColorAndType GetPieceColorAndType(Coordinate from) const;
		bool TryInitPiece(Coordinate from);
		bool IsCoordinateInPossibleMoves(Coordinate coord);
		bool TryMovePiece(Coordinate to);
	};
}
