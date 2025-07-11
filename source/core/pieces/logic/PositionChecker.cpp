#include "PositionChecker.h"

#include "core/logic/Coordinate.h"
#include "core/logic/Sizes.h"

bool Chess::PositionChecker::IsPositionValid(const Coordinate& position)
{
    return position.GetFile() >= 'A' && position.GetFile() < 'A' + CHESSBOARD_SIZE && position.GetRank() >= 1 && position.GetRank() <= CHESSBOARD_SIZE;
}
