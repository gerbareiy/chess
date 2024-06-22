#pragma once

#include "../IPiece.h"
#include "IKing.h"
#include "MoveChecker.h"
#include "PieceFinder.h"

#include <memory>
#include <vector>

namespace Chess
{
	class CheckChecker
	{
	public:
		bool IsCheck(std::shared_ptr<IKing> king, const std::vector<std::shared_ptr<IPiece>>& piece);
		bool IsCheck(const Chess::ePieceColor& kingColor, const std::vector<std::shared_ptr<IPiece>>& piecesOnBoard);
	};
}
