#pragma once

#include "logic/Coordinate.h"
#include "pieces/IPiece.h"
#include "pieces/logic/MoveChecker.h"
#include "pieces/logic/PieceColorAndType.h"

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

	public:
		const std::vector<std::shared_ptr<IPiece>>& get_EatenPieces() const;

	private:
		std::shared_ptr<Chess::IPiece> GetPiece(Coordinate from) const;
		bool IsValidMove(Coordinate to);
		void Take(size_t indexOnBoard);

	public:
		PieceColorAndType GetPieceColorAndType(Coordinate from) const;
		bool IsCoordinateInPossibleMoves(Coordinate coord);
		bool TryInitPiece(Coordinate from);
		bool TryMovePiece(Coordinate to);
	};
}
