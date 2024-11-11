#include "MoveCheckerFactory.h"

#include "../Bishop.h"
#include "../BishopChecker.h"
#include "../King.h"
#include "../KingChecker.h"
#include "../Knight.h"
#include "../KnightChecker.h"
#include "../Pawn.h"
#include "../PawnChecker.h"
#include "../Piece.h"
#include "../Queen.h"
#include "../QueenChecker.h"
#include "../Rook.h"
#include "../RookChecker.h"
#include "../../logic/eError.h"
#include "../../logic/ErrorConverter.h"

std::shared_ptr<Chess::IMoveChecker> Chess::MoveCheckerFactory::Create(const std::shared_ptr<Piece>& piece) const
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
