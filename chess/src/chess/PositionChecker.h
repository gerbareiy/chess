#pragma once

#include "Coordinate.h"

namespace Chess
{
	class PositionChecker
	{
	public:
		static bool IsPositionValid(const Coordinate& position);
	};
}
