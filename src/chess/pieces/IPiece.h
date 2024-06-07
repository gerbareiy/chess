#pragma once

#include "PieceColorAndType.h"
#include "../Coordinate.h"

namespace Chess
{
	class IPiece
	{
	public:
		virtual ~IPiece() = default;

	public:
		virtual PieceColorAndType get_ColorAndType() const = 0;
		virtual Coordinate get_Position() const = 0;

	public:
		virtual void Move(Coordinate to) = 0;
	};
}
