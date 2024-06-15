#pragma once

#include "logic/ePieceColor.h"
#include "IPiece.h"
#include "logic/PieceColorAndType.h"
#include "../Coordinate.h"

namespace Chess
{
	class King : public IPiece
	{
	private:
		bool m_canMakeCastling = true;
		PieceColorAndType m_colorAndType{};
		bool m_isCheck = false;
		Coordinate m_position{};
		
	public:
		King(ePieceColor color);

	public:
		bool get_CanMakeCasting() const;
		PieceColorAndType get_ColorAndType() const override;
		bool get_IsCheck() const;
		Coordinate get_Position() const override;

	private:
		void DisableCastling();

	public:
		void Move(Coordinate to) override;
	};
}
