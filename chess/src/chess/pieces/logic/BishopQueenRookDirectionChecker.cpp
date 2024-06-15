#include "BishopQueenRookDirectionChecker.h"

#include "../../PositionChecker.h"

std::vector<Chess::Coordinate> Chess::BishopQueenRookDirectionChecker::FindPossibleMoves(const std::shared_ptr<PieceFinder>& finder, const std::shared_ptr<IPiece>& current, std::pair <int, int> coordinateIncrement)
{
	std::vector<Coordinate> moves;
	auto currentCoordinate = current->get_Position();

	while (true)
	{
		currentCoordinate.set_File(currentCoordinate.get_File() + coordinateIncrement.first);
		currentCoordinate.set_Rank(currentCoordinate.get_Rank() + coordinateIncrement.second);

		if (!PositionChecker::IsPositionValid(currentCoordinate))
		{
			break;
		}

		const auto found = finder->Find(currentCoordinate);

		if (found)
		{
			if (found->get_ColorAndType().get_Color() != current->get_ColorAndType().get_Color())
			{
				moves.emplace_back(currentCoordinate);
			}

			break;
		}

		moves.emplace_back(currentCoordinate);
	}

	return moves;
}
