#pragma once

#include "IMoveChecker.h"

#include <memory>

namespace Chess
{
	class Piece;

	class MoveCheckerFactory
	{
	public:
		 std::shared_ptr<IMoveChecker> Create(std::shared_ptr<Piece> const& piece) const;
	};
}
