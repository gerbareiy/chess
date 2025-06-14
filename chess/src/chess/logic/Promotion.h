#pragma once

#include <memory>
#include <vector>

namespace Chess
{
	class Pawn;
	class Piece;
	struct Coordinate;

	class Promotion
	{
	public:
		static void PromoteConditionally(std::shared_ptr<Pawn> const& pawn, std::vector<std::shared_ptr<Piece>>& piecesOnBoard);
		static void PromoteConditionally(Coordinate const& pawnPosition, std::vector<std::shared_ptr<Piece>>& piecesOnBoard);
	};
}
