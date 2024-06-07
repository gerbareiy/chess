#pragma once

#include "pieces/PieceColorAndType.h"
#include "pieces/IPiece.h"
#include "Coordinate.h"

#include <memory>
#include <vector>

namespace Chess
{
	class Chessboard
	{
	private:
		std::vector<std::shared_ptr<IPiece>> m_piecesOnBoard{};
		std::vector<std::shared_ptr<IPiece>> m_eatenPieces{};

	public:
		Chessboard();

	private:
		std::shared_ptr<Chess::IPiece> GetPiece(Coordinate from) const;
		bool IsValidMove(std::shared_ptr<IPiece> piece, Coordinate to) const;

	public:
		PieceColorAndType GetPieceColorAndType(Coordinate from) const;
		bool TryMovePiece(Coordinate from, Coordinate to);
	};
}
