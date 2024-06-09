#pragma once

#include "ePieceColor.h"
#include "IPiece.h"
#include "PieceColorAndType.h"
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
		~Queen() {};

	public:
		PieceColorAndType get_ColorAndType() const override;
		Coordinate get_Position() const override;

	public:
		void Move(Coordinate to) override;
	};
}
