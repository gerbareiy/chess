#pragma once

#include "../Piece.h"
#include "../../logic/Coordinate.h"

#include <memory>
#include <vector>

namespace Chess
{
	class IMoveChecker
	{
	public:
		virtual ~IMoveChecker() = default;

	public:
		virtual std::vector<Coordinate> GetMoves(const std::shared_ptr<Piece>& piece, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const = 0;
	};
}
