#pragma once

#include "logic/ePieceColor.h"
#include "IPiece.h"
#include "logic/PieceColorAndType.h"
#include "../Coordinate.h"

#include <vector>

namespace Chess
{
	class Queen : public IPiece
	{
	private:
		PieceColorAndType m_colorAndType{};
		Coordinate m_position{};

	public:
		Queen(ePieceColor color);

	public:
		PieceColorAndType get_ColorAndType() const override;
		Coordinate get_Position() const override;

	public:
		void Move(Coordinate to) override;
	};
}
