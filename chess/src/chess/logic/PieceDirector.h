#pragma once

#include "../LableDisplayer.h"

#include <boost/signals2.hpp>

#include <memory>
#include <vector>

namespace Chess
{
	class PieceSignalDirector;
	class Promotion;
	class Piece;
	struct Coordinate;
	struct PieceColorAndType;

	class PieceDirector
	{
	private:
		std::shared_ptr<Piece> m_currentPiece;
		std::vector<std::shared_ptr<Piece>> m_eatenPieces;
		std::unique_ptr<LableDisplayer> m_inputDisplayer;
		bool m_isCheck = false;
		std::vector<std::shared_ptr<Piece>>& m_piecesOnBoard;
		std::shared_ptr<Promotion> m_promotion;
		std::shared_ptr<PieceSignalDirector> m_signalDirector;

	public:
		PieceDirector(std::vector<std::shared_ptr<Piece>>& piecesOnBoard, std::shared_ptr<PieceSignalDirector> const& signalDirector);

	private:
		void Take(int indexOnBoard);

	public:
		std::shared_ptr<Piece> const& GetCurrentPiece() const;
		std::vector<std::shared_ptr<Piece>> const& GetEatenPieces() const;
		bool GetIsCheck() const;
		std::vector<std::shared_ptr<Piece>> const& GetPiecesOnBoard() const;

		PieceColorAndType GetPieceColorAndType(Coordinate const& frFom) const;
		std::shared_ptr<Chess::Piece> GetPiece(Coordinate const& from) const;
		void InitCurrentPiece(Coordinate const& from);
		void MovePiece(Coordinate const& to, boost::signals2::signal<void()> const& signalChessboardUndated);
	};
}
