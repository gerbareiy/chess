#pragma once

namespace Chess
{
	struct Coordinate;

	class PositionChecker
	{
	public:
		static bool IsPositionValid(Coordinate const& position);
	};
}
