#pragma once

#include "IMoveChecker.h"
#include "../IPiece.h"

#include <memory>

namespace Chess
{
	class MoveCheckerFactory
	{
	public:
		 std::shared_ptr<IMoveChecker> Create(const std::shared_ptr<IPiece>& piece);
	};
}
