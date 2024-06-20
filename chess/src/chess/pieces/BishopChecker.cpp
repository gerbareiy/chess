#include "BishopChecker.h"

#include "logic/BishopQueenRookDirectionChecker.h"
#include "logic/PieceFinder.h"
#include "../logic/Counts.h"
#include "../logic/eError.h"
#include "../logic/ErrorConverter.h"

#include <memory>

std::vector<Chess::Coordinate> Chess::BishopChecker::FindPossibleMoves(const std::shared_ptr<Bishop>& bishop, const std::vector<std::shared_ptr<IPiece>>& piecesOnBoard)
{
	std::vector<Coordinate> moves;
	moves.reserve(COUNT_OF_BISHOP_WAYS);

	const auto finder = std::make_shared<PieceFinder>(piecesOnBoard);
	auto coordinate = bishop->get_Position();

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

std::vector<Chess::Coordinate> Chess::BishopChecker::GetPossibleMoves(const std::shared_ptr<IPiece>& piece, const std::vector<std::shared_ptr<IPiece>>& piecesOnBoard)
{
	if (!piece || typeid(*piece) != typeid(Bishop) || piece->get_ColorAndType().get_Type() != ePieceType::BISHOP)
	{
		return std::vector<Coordinate>();
	}

	return FindPossibleMoves(std::static_pointer_cast<Bishop>(piece), piecesOnBoard);
}