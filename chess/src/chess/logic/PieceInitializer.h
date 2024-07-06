#pragma once

#include "PieceSignalDirector.h"
#include "../pieces/Piece.h"

#include <memory>
#include <vector>

namespace Chess
{
	class PieceInitializer
	{
	public:
		std::vector<std::shared_ptr<Piece>> InitStandartBoard(std::shared_ptr<PieceSignalDirector> signalDirector) const;
	};
}

