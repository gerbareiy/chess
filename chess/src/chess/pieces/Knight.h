#pragma once

#include "logic/ePieceColor.h"
#include "IPiece.h"
#include "logic/PieceColorAndType.h"
#include "../Coordinate.h"

namespace Chess
{
	class Knight : public IPiece
	{
	private:
		PieceColorAndType m_colorAndType{};
		Coordinate m_position{};

	public:
		Knight(ePieceColor pieceColor, int orderNumber);

	public:
		PieceColorAndType get_ColorAndType() const override;
		Coordinate get_Position() const override;

	public:
		void Move(Coordinate to) override;
	};
}
