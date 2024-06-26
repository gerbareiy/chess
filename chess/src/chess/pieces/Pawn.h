#pragma once

#include "IPiece.h"
#include "logic/ePieceColor.h"
#include "logic/PieceColorAndType.h"
#include "../logic/Coordinate.h"
#include "../logic/PieceSignalDirector.h"

#include "memory"

namespace Chess
{
	class Pawn : public IPiece
	{
	private:
		static constexpr int m_MAX_POSSIBLE_COUNT_MOVES = 2;

	private:
		bool m_canEnPassant = false;
		PieceColorAndType m_colorAndType;
		bool m_isOnPawnFirstMove = false;
		bool m_isNotMoved = true;
		Coordinate m_position;

	public:
		Pawn(ePieceColor color, char file);
		Pawn(ePieceColor color, char file, const std::shared_ptr<PieceSignalDirector>& signalDirector);
		Pawn(ePieceColor color, Coordinate coordinate);
		Pawn(ePieceColor color, Coordinate coordinate, const std::shared_ptr<PieceSignalDirector>& signalDirector);

	public:
		bool get_CanEnPassant() const;
		PieceColorAndType get_ColorAndType() const override;
		bool get_IsNotMoved() const;
		Coordinate get_Position() const override;

	private:
		void LostEnPassant();

	public:
		void Move(Coordinate to, bool isRealMove = true) override;
	};
}
