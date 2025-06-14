#include "BishopChecker.h"

#include "Bishop.h"
#include "logic/BishopQueenRookDirectionChecker.h"
#include "logic/ePieceType.h"
#include "logic/PieceColorAndType.h"
#include "logic/PieceFinder.h"
#include "../logic/Coordinate.h"
#include "../logic/Counts.h"

#include <memory>

std::vector<Chess::Coordinate> Chess::BishopChecker::FindPossibleMoves(std::shared_ptr<Bishop> const& bishop, std::vector<std::shared_ptr<Piece>> const& piecesOnBoard) const
{
	std::vector<Coordinate> moves;
	moves.reserve(COUNT_OF_BISHOP_WAYS);

	auto const finder = std::make_shared<PieceFinder>(piecesOnBoard);
	auto coordinate = bishop->GetPosition();

	auto first = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, bishop, { -1, -1 });
	auto second = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, bishop, { -1, 1 });
	auto third = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, bishop, { 1, -1 });
	auto fourth = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, bishop, { 1, 1 });

	moves.insert(moves.end(), first.begin(), first.end());
	moves.insert(moves.end(), second.begin(), second.end());
	moves.insert(moves.end(), third.begin(), third.end());
	moves.insert(moves.end(), fourth.begin(), fourth.end());

	return moves;
}

std::vector<Chess::Coordinate> Chess::BishopChecker::GetMoves(std::shared_ptr<Piece> const& piece, std::vector<std::shared_ptr<Piece>> const& piecesOnBoard) const
{
	if (!piece || typeid(*piece) != typeid(Bishop) || piece->GetColorAndType().GetType() != ePieceType::BISHOP)
	{
		return std::vector<Coordinate>();
	}

	return FindPossibleMoves(std::static_pointer_cast<Bishop>(piece), piecesOnBoard);
}