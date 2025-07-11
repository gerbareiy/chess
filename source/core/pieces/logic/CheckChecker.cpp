#include "CheckChecker.h"

#include "MoveCheckerFactory.h"
#include "PieceColorAndType.h"
#include "PieceFinder.h"
#include "core//pieces/Piece.h"
#include "core/logic/Coordinate.h"
#include "core/pieces/King.h"

bool Chess::CheckChecker::IsCheck(const std::shared_ptr<King>& king, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard)
{
    for (const auto& piece : piecesOnBoard)
    {
        if (piece->GetColorAndType().GetColor() == king->GetColorAndType().GetColor())
        {
            continue;
        }

        const auto moveChecker = std::make_unique<MoveCheckerFactory>()->Create(piece);
        auto       moves       = moveChecker->GetMoves(piece, piecesOnBoard);

        for (const auto& move : moves)
        {
            if (move == king->GetPosition())
            {
                return true;
            }
        }
    }

    return false;
}

bool Chess::CheckChecker::IsCheck(const ePieceColor& kingColor, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard)
{
    for (const auto& piece : piecesOnBoard)
    {
        const auto king = std::dynamic_pointer_cast<King>(piece);

        if (king && king->GetColorAndType().GetColor() == kingColor)
        {
            return IsCheck(king, piecesOnBoard);
        }
    }

    return false;
}
