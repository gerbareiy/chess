#pragma once

#include "ePieceColor.h"
#include "IPiece.h"
#include "PieceColorAndType.h"
#include "../Coordinate.h"

#include <vector>

namespace Chess
{
	class Pawn : public IPiece
	{
	private:
		static constexpr int m_MAX_POSSIBLE_COUNT_MOVES = 2;

	private:
		bool m_canEnPassant = false;
		PieceColorAndType m_colorAndType{};
		bool m_isNotMoved = true;
		Coordinate m_position{};

	public:
		Pawn(ePieceColor color, char file);
		~Pawn() {};

	public:
		bool get_CanEnPassant() const { return m_canEnPassant; };
		PieceColorAndType get_ColorAndType() const override { return m_colorAndType; };
		bool get_IsNotMoved() const { return m_isNotMoved; };
		Coordinate get_Position() const override { return m_position; }

	private:
		void LostEnPassant() { m_canEnPassant = false; }

	public:
		void Move(Coordinate to) override;
	};
}