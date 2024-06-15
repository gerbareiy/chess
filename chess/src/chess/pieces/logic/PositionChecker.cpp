#include "PositionChecker.h"

#include "../../logic/Sizes.h"

bool Chess::PositionChecker::IsPositionValid(const Coordinate& position)
{
	return position.get_File() >= 'A'
		&& position.get_File() < 'A' + CHESSBOARD_SIZE
		&& position.get_Rank() >= 1
		&& position.get_Rank() <= CHESSBOARD_SIZE;
}
