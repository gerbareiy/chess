#include "MoveCheckerFactory.h"

#include "core/logic/ErrorConverter.h"
#include "core/logic/eError.h"
#include "core/pieces/Bishop.h"
#include "core/pieces/BishopChecker.h"
#include "core/pieces/King.h"
#include "core/pieces/KingChecker.h"
#include "core/pieces/Knight.h"
#include "core/pieces/KnightChecker.h"
#include "core/pieces/Pawn.h"
#include "core/pieces/PawnChecker.h"
#include "core/pieces/Piece.h"
#include "core/pieces/Queen.h"
#include "core/pieces/QueenChecker.h"
#include "core/pieces/Rook.h"
#include "core/pieces/RookChecker.h"

std::shared_ptr<Chess::IMoveChecker> Chess::MoveCheckerFactory::Create(const std::shared_ptr<Piece>& piece)
{
    if (typeid(*piece) == typeid(Bishop))
    {
        return std::make_shared<BishopChecker>();
    }
    if (typeid(*piece) == typeid(King))
    {
        return std::make_shared<KingChecker>();
    }
    if (typeid(*piece) == typeid(Knight))
    {
        return std::make_shared<KnightChecker>();
    }
    if (typeid(*piece) == typeid(Pawn))
    {
        return std::make_shared<PawnChecker>();
    }
    if (typeid(*piece) == typeid(Queen))
    {
        return std::make_shared<QueenChecker>();
    }
    if (typeid(*piece) == typeid(Rook))
    {
        return std::make_shared<RookChecker>();
    }

    throw std::invalid_argument(ErrorConverter::ToString(eError::NOT_CORRECT_PIECE));
}
