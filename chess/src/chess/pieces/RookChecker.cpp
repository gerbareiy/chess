#include "RookChecker.h"

#include "logic/BishopQueenRookDirectionChecker.h"
#include "logic/ePieceType.h"
#include "logic/PieceColorAndType.h"
#include "logic/PieceFinder.h"
#include "logic/PositionChecker.h"
#include "Rook.h"
#include "../logic/Coordinate.h"
#include "../logic/Counts.h"
#include "../logic/eError.h"
#include "../logic/ErrorConverter.h"


#include <memory>

std::vector<Chess::Coordinate> Chess::RookChecker::FindPossibleMoves(const std::shared_ptr<Rook>& rook, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const
{
	std::vector<Coordinate> moves;
	moves.reserve(COUNT_OF_ROOK_WAYS);

	const auto finder = std::make_shared<PieceFinder>(piecesOnBoard);
	auto coordinate = rook->get_Position();

	auto first = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, rook, { -1, 0 });
	auto second = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, rook, { 1, 0 });
	auto third = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, rook, { 0, -1 });
	auto fourth = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, rook, { 0, 1 });

	moves.insert(moves.end(), first.begin(), first.end());
	moves.insert(moves.end(), second.begin(), second.end());
	moves.insert(moves.end(), third.begin(), third.end());
	moves.insert(moves.end(), fourth.begin(), fourth.end());

	return moves;
}

std::vector<Chess::Coordinate> Chess::RookChecker::GetMoves(const std::shared_ptr<Piece>& piece, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const
{
	if (!piece || typeid(*piece) != typeid(Rook) || piece->get_ColorAndType().get_Type() != ePieceType::ROOK)
	{
		return std::vector<Coordinate>();
	}

	return FindPossibleMoves(std::static_pointer_cast<Rook>(piece), piecesOnBoard);
}
