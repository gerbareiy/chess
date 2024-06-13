#include "BishopChecker.h"

#include "PieceFinder.h"
#include "../eError.h"
#include "../ErrorConverter.h"
#include "../Counts.h"
#include "../PositionChecker.h"

#include <memory>
#include <stdexcept>

std::vector<Chess::Coordinate> Chess::BishopChecker::FindPossibleMoves(const std::shared_ptr<Bishop>& bishop, const std::vector<std::shared_ptr<IPiece>>& piecesOnBoard)
{
	std::vector<Coordinate> moves;
	moves.reserve(COUNT_OF_BISHOP_WAYS);

	const auto finder = std::make_shared<PieceFinder>(piecesOnBoard);
	auto coordinate = bishop->get_Position();

	auto checkDirection = [&](int fileIncrement, int rankIncrement)
		{
			Coordinate current = bishop->get_Position();

			while (true)
			{
				current.set_File(current.get_File() + fileIncrement);
				current.set_Rank(current.get_Rank() + rankIncrement);

				if (!PositionChecker::IsPositionValid(current))
				{
					break;
				}

				const auto found = finder->Find(current);
				if (found)
				{
					if (found->get_ColorAndType().get_Color() != bishop->get_ColorAndType().get_Color())
					{
						moves.emplace_back(current);
					}

					break;
				}

				moves.emplace_back(current);
			}
		};

	checkDirection(-1, -1);
	checkDirection(-1, 1);
	checkDirection(1, -1);
	checkDirection(1, 1);

	return moves;
}

std::vector<Chess::Coordinate> Chess::BishopChecker::GetPossibleMoves(const std::shared_ptr<IPiece>& piece, const std::vector<std::shared_ptr<IPiece>>& piecesOnBoard)
{
	if (!piece || typeid(*piece) != typeid(Bishop) || piece->get_ColorAndType().get_Type() != ePieceType::BISHOP)
	{
		throw std::out_of_range(ErrorConverter::ToString(eError::NOT_CORRECT_PIECE));
	}

	return FindPossibleMoves(std::static_pointer_cast<Bishop>(piece), piecesOnBoard);
}