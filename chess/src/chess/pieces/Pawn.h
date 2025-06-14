#pragma once

#include "Piece.h"
#include "logic/ePieceColor.h"

#include "memory"

namespace Chess
{
	class PieceSignalDirector;
	struct Coordinate;
	struct PieceColorAndType;

	class Pawn : public Piece
	{
	private:
		static constexpr int m_MAX_POSSIBLE_COUNT_MOVES = 2;

	private:
		bool m_canEnPassant = false;
		bool m_isOnPawnFirstMove = false;
		bool m_isNotMoved = true;

	public:
		Pawn(ePieceColor color, char file);
		Pawn(ePieceColor color, char file, std::shared_ptr<PieceSignalDirector> const& signalDirector);
		Pawn(ePieceColor color, Coordinate const& coordinate);
		Pawn(ePieceColor color, Coordinate const& coordinate, std::shared_ptr<PieceSignalDirector> const& signalDirector);

	private:
		void LostEnPassant();
		void MakeTracking(std::shared_ptr<Chess::PieceSignalDirector> const& signalDirector);

	public:
		bool GetCanEnPassant() const;
		bool GetIsNotMoved() const;

		void Move(Coordinate to, bool isRealMove = true) override;
	};
}
