#pragma once

#include "ePieceColor.h"

#include <memory>
#include <vector>

namespace Chess
{
	class MoveChecker;
	class Piece;
	class PieceFinder;
	class King;

	class CheckChecker
	{
	public:
		static bool IsCheck(std::shared_ptr<King> const& king, std::vector<std::shared_ptr<Piece>> const& piece);
		static bool IsCheck(Chess::ePieceColor const& kingColor, std::vector<std::shared_ptr<Piece>> const& piecesOnBoard);
	};
}
