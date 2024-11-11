#pragma once

#include <memory>
#include <vector>

namespace Chess
{
	class Piece;
	class PieceSignalDirector;

	class PieceInitializer
	{
	public:
		std::vector<std::shared_ptr<Piece>> InitStandartBoard(std::shared_ptr<PieceSignalDirector> signalDirector) const;
	};
}

