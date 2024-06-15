#include "QueenChecker.h"

#include "logic/BishopQueenRookDirectionChecker.h"
#include "logic/PieceFinder.h"
#include "../logic/eError.h"
#include "../logic/ErrorConverter.h"
#include "../logic/Counts.h"
#include "logic/PositionChecker.h"

#include <memory>
#include <stdexcept>

std::vector<Chess::Coordinate> Chess::QueenChecker::FindPossibleMoves(const std::shared_ptr<Queen>& queen, const std::vector<std::shared_ptr<IPiece>>& piecesOnBoard)
{
	std::vector<Coordinate> moves;
	moves.reserve(COUNT_OF_QUEEN_WAYS);

	const auto finder = std::make_shared<PieceFinder>(piecesOnBoard);
	auto coordinate = queen->get_Position();

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

std::vector<Chess::Coordinate> Chess::QueenChecker::GetPossibleMoves(const std::shared_ptr<IPiece>& piece, const std::vector<std::shared_ptr<IPiece>>& piecesOnBoard)
{
	if (!piece || typeid(*piece) != typeid(Queen) || piece->get_ColorAndType().get_Type() != ePieceType::QUEEN)
	{
		throw std::invalid_argument(ErrorConverter::ToString(eError::NOT_CORRECT_PIECE));
	}

	return FindPossibleMoves(std::static_pointer_cast<Queen>(piece), piecesOnBoard);
}
