#pragma once

#include "logic/ePieceColor.h"
#include "IPiece.h"
#include "logic/PieceColorAndType.h"
#include "../logic/Coordinate.h"

namespace Chess
{
	class Queen : public IPiece
	{
	private:
		PieceColorAndType m_colorAndType;
		Coordinate m_position;

	public:
		Queen(ePieceColor color);
		Queen(ePieceColor color, Coordinate coordinate);

	public:
		PieceColorAndType get_ColorAndType() const override;
		Coordinate get_Position() const override;

	public:
		void Move(Coordinate to, bool isRealMove = true) override;
	};
}
