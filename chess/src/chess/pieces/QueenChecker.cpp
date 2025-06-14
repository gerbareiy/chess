#include "QueenChecker.h"

#include "logic/BishopQueenRookDirectionChecker.h"
#include "logic/ePieceType.h"
#include "logic/PieceColorAndType.h"
#include "logic/PieceFinder.h"
#include "../logic/Coordinate.h"
#include "../logic/Counts.h"

#include <memory>

std::vector<Chess::Coordinate> Chess::QueenChecker::FindPossibleMoves(std::shared_ptr<Queen> const& queen, std::vector<std::shared_ptr<Piece>> const& piecesOnBoard) const
{
	std::vector<Coordinate> moves;
	moves.reserve(COUNT_OF_QUEEN_WAYS);

	auto const finder = std::make_shared<PieceFinder>(piecesOnBoard);
	auto coordinate = queen->GetPosition();

	auto first = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, queen, { -1, -1 });
	auto second = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, queen, { -1, 1 });
	auto third = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, queen, { 1, -1 });
	auto fourth = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, queen, { 1, 1 });
	auto fifth = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, queen, { -1, 0 });
	auto sixth = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, queen, { 1, 0 });
	auto seventh = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, queen, { 0, -1 });
	auto eighth = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, queen, { 0, 1 });

	moves.insert(moves.end(), first.begin(), first.end());
	moves.insert(moves.end(), second.begin(), second.end());
	moves.insert(moves.end(), third.begin(), third.end());
	moves.insert(moves.end(), fourth.begin(), fourth.end());
	moves.insert(moves.end(), fifth.begin(), fifth.end());
	moves.insert(moves.end(), sixth.begin(), sixth.end());
	moves.insert(moves.end(), seventh.begin(), seventh.end());
	moves.insert(moves.end(), eighth.begin(), eighth.end());

	return moves;
}

std::vector<Chess::Coordinate> Chess::QueenChecker::GetMoves(std::shared_ptr<Piece> const& piece, std::vector<std::shared_ptr<Piece>> const& piecesOnBoard) const
{
	if (!piece || typeid(*piece) != typeid(Queen) || piece->GetColorAndType().GetType() != ePieceType::QUEEN)
	{
		return std::vector<Coordinate>();
	}

	return FindPossibleMoves(std::static_pointer_cast<Queen>(piece), piecesOnBoard);
}
