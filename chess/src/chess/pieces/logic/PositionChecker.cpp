#include "PositionChecker.h"

#include "../../logic/Coordinate.h"
#include "../../logic/Sizes.h"

bool Chess::PositionChecker::IsPositionValid(Coordinate const& position)
{
	return position.GetFile() >= 'A'
		&& position.GetFile() < 'A' + CHESSBOARD_SIZE
		&& position.GetRank() >= 1
		&& position.GetRank() <= CHESSBOARD_SIZE;
}
