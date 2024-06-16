#pragma once

#include "IPiece.h"
#include "logic/ePieceColor.h"
#include "logic/IKing.h"
#include "logic/PieceColorAndType.h"
#include "../logic/Coordinate.h"

namespace Chess
{
	class King : public IPiece, public IKing
	{
	private:
		bool m_canMakeCastling = true;
		PieceColorAndType m_colorAndType{};
		bool m_isCheck = false;
		Coordinate m_position{};
		
	public:
		King(ePieceColor color);

	public:
		bool get_CanMakeCastling() const;
		PieceColorAndType get_ColorAndType() const override;
		bool get_IsCheck() const override;
		Coordinate get_Position() const override;

	private:
		void DisableCastling();

	public:
		void Move(Coordinate to) override;
	};
}
