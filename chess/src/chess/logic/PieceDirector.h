#pragma once

#include "Coordinate.h"
#include "PieceSignalDirector.h"
#include "../pieces/IPiece.h"

#include <boost/signals2.hpp>

#include <memory>
#include <vector>

namespace Chess
{
	class PieceDirector
	{
	private:
		std::vector<std::shared_ptr<IPiece>> m_eatenPieces;
		std::shared_ptr<IPiece> m_currentPiece;
		std::vector<std::shared_ptr<IPiece>> m_piecesOnBoard;
		std::shared_ptr<PieceSignalDirector> m_signalDirector;

	public:
		PieceDirector(std::vector<std::shared_ptr<IPiece>> piecesOnBoard, std::shared_ptr<PieceSignalDirector> signalDirector);

	public:
		const std::shared_ptr<IPiece>& get_CurrentPiece() const;
		const std::vector<std::shared_ptr<IPiece>>& get_EatenPieces() const;
		const std::vector<std::shared_ptr<IPiece>>& get_PiecesOnBoard() const;

	private:
		void Take(size_t indexOnBoard);

	public:
		PieceColorAndType GetPieceColorAndType(const Coordinate& from) const;
		std::shared_ptr<Chess::IPiece> GetPiece(const Coordinate& from) const;
		void InitCurrentPiece(const Coordinate& from);
		void MovePiece(const Coordinate& to);
	};
}
