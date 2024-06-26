#pragma once

#include "Coordinate.h"
#include "PieceSignalDirector.h"
#include "Promotion.h"
#include "../pieces/IPiece.h"
#include "../LableDisplayer.h"

#include <boost/signals2.hpp>

#include <memory>
#include <vector>

namespace Chess
{
	class PieceDirector
	{
	private:
		std::shared_ptr<IPiece> m_currentPiece;
		std::vector<std::shared_ptr<IPiece>> m_eatenPieces;
		std::unique_ptr<LableDisplayer> m_inputDisplayer;
		bool m_isCheck = false;
		std::vector<std::shared_ptr<IPiece>>& m_piecesOnBoard;
		std::shared_ptr<Promotion> m_promotion;
		std::shared_ptr<PieceSignalDirector> m_signalDirector;

	public:
		PieceDirector(std::vector<std::shared_ptr<IPiece>>& piecesOnBoard, const std::shared_ptr<PieceSignalDirector>& signalDirector);

	public:
		const std::shared_ptr<IPiece>& get_CurrentPiece() const;
		const std::vector<std::shared_ptr<IPiece>>& get_EatenPieces() const;
		bool get_IsCheck() const;
		const std::vector<std::shared_ptr<IPiece>>& get_PiecesOnBoard() const;

	private:
		void Take(int indexOnBoard);

	public:
		PieceColorAndType GetPieceColorAndType(const Coordinate& from) const;
		std::shared_ptr<Chess::IPiece> GetPiece(const Coordinate& from) const;
		void InitCurrentPiece(const Coordinate& from);
		void MovePiece(const Coordinate& to, const boost::signals2::signal<void()>& signalChessboardUndated);
	};
}
