#pragma once

#include "IPiece.h"
#include "logic/ePieceColor.h"
#include "logic/PieceColorAndType.h"
#include "../logic/Coordinate.h"

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
