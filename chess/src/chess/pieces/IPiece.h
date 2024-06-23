#pragma once

#include "logic/PieceColorAndType.h"
#include "../logic/Coordinate.h"

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
		virtual void Move(Coordinate to, bool isRealMove = true) = 0;
	};
}
