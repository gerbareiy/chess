#pragma once

#include "ePieceColor.h"
#include "IPiece.h"
#include "King.h"
#include "PieceColorAndType.h"
#include "../Coordinate.h"

namespace Chess
{
	class Rook : public IPiece
	{
	private:
		bool m_canMakeCastling = false;
		PieceColorAndType m_colorAndType{};
		Coordinate m_position{};

	public:
		Rook(ePieceColor pieceColor, int orderNumber);
		~Rook() {};

	public:
		bool get_CanMakeCasting() const;
		PieceColorAndType get_ColorAndType() const override;
		Coordinate get_Position() const override;

	private:
		void DisableCastling();

	public:
		void Move(Coordinate to) override;
		void OnCasting(King king);
	};
}
