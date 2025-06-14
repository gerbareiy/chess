#include "BishopQueenRookDirectionChecker.h"

#include "PieceColorAndType.h"
#include "PieceFinder.h"
#include "PositionChecker.h"
#include "../Piece.h"
#include "../../logic/Coordinate.h"

std::vector<Chess::Coordinate> Chess::BishopQueenRookDirectionChecker::FindPossibleMoves(std::shared_ptr<PieceFinder> const& finder, std::shared_ptr<Piece> const& current, std::pair <int, int> coordinateIncrement)
{
	std::vector<Coordinate> moves;
	auto currentCoordinate = current->GetPosition();

	while (true)
	{
		currentCoordinate.SetFile(currentCoordinate.GetFile() + coordinateIncrement.first);
		currentCoordinate.SetRank(currentCoordinate.GetRank() + coordinateIncrement.second);

		if (!PositionChecker::IsPositionValid(currentCoordinate))
		{
			break;
		}

		auto const found = finder->Find(currentCoordinate);

		if (found)
		{
			if (found->GetColorAndType().GetColor() != current->GetColorAndType().GetColor())
			{
				moves.emplace_back(currentCoordinate);
			}

			break;
		}

		moves.emplace_back(currentCoordinate);
	}

	return moves;
}
