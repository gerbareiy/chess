#include "MoveChecker.h"

#include "Bishop.h"
#include "BishopChecker.h"
#include "King.h"
#include "KingChecker.h"
#include "Knight.h"
#include "KnightChecker.h"
#include "Pawn.h"
#include "PawnChecker.h"
#include "Queen.h"
#include "QueenChecker.h"
#include "Rook.h"
#include "RookChecker.h"
#include "../Coordinate.h"

#include <memory>
#include <vector>

std::vector<Chess::Coordinate> Chess::MoveChecker::GetPossibleMoves(const std::shared_ptr<IPiece>& piece, const std::vector<std::shared_ptr<IPiece>>& piecesOnBoard)
{
    if (typeid(*piece) == typeid(Bishop))
    {
        return std::make_unique<BishopChecker>()->GetPossibleMoves(piece, piecesOnBoard);
    }
    if (typeid(*piece) == typeid(King))
    {
        return std::make_unique<KingChecker>()->GetPossibleMoves(piece, piecesOnBoard);
    }
    if (typeid(*piece) == typeid(Knight))
    {
        return std::make_unique<KnightChecker>()->GetPossibleMoves(piece, piecesOnBoard);
    }
    if (typeid(*piece) == typeid(Pawn))
    {
        return std::make_unique<PawnChecker>()->GetPossibleMoves(piece, piecesOnBoard);
    }
    if (typeid(*piece) == typeid(Rook))
    {
        return std::make_unique<RookChecker>()->GetPossibleMoves(piece, piecesOnBoard);
    }
    return std::vector<Coordinate>();
}
