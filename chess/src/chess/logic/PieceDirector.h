#pragma once

#include "Coordinate.h"
#include "../pieces/IPiece.h"

#include <memory>
#include <vector>

namespace Chess
{
	class PieceDirector
	{
	private:
		std::vector<std::shared_ptr<IPiece>> m_eatenPieces{};
		std::shared_ptr<IPiece> m_currentPiece{};
		std::vector<std::shared_ptr<IPiece>> m_piecesOnBoard{};

	public:
		PieceDirector();

	public:
		const std::shared_ptr<IPiece>& get_CurrentPiece() const;
		const std::vector<std::shared_ptr<IPiece>>& get_EatenPieces() const;
		const std::vector<std::shared_ptr<IPiece>>& get_PiecesOnBoard() const;

	private:
		void Take(size_t indexOnBoard);

	public:
		std::shared_ptr<Chess::IPiece> GetPiece(const Coordinate& from) const;
		void InitCurrantPiece(const Coordinate& from);
		void MovePiece(const Coordinate& to);
	};
}
