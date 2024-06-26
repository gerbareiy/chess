#pragma once

#include "../pieces/Pawn.h"
#include "../pieces/IPiece.h"

#include <memory>
#include <vector>

namespace Chess
{
	class Promotion
	{
	public:
		void PromoteConditionally(std::shared_ptr<Pawn> pawn, std::vector<std::shared_ptr<IPiece>>& piecesOnBoard);
	};
}
