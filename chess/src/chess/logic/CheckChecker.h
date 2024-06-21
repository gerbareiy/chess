#pragma once

#include "../pieces/IPiece.h"
#include "../pieces/logic/IKing.h"
#include "../pieces/logic/MoveChecker.h"
#include "../pieces/logic/PieceFinder.h"

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
