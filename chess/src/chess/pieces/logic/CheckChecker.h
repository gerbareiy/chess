#pragma once

#include "MoveChecker.h"
#include "PieceFinder.h"
#include "../Piece.h"
#include "../King.h"

#include <memory>
#include <vector>

namespace Chess
{
	class CheckChecker
	{
	public:
		bool IsCheck(std::shared_ptr<King> king, const std::vector<std::shared_ptr<Piece>>& piece) const;
		bool IsCheck(const Chess::ePieceColor& kingColor, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const;
	};
}
