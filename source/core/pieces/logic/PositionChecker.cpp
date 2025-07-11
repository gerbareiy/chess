#include "PositionChecker.h"

#include "core/logic/Coordinate.h"
#include "core/logic/Sizes.h"

bool Chess::PositionChecker::IsPositionValid(const Coordinate& position)
{
    return position.file >= 'A' && position.file < 'A' + CHESSBOARD_SIZE && position.rank >= 1 && position.rank <= CHESSBOARD_SIZE;
}
