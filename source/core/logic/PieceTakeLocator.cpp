#include "PieceTakeLocator.h"

#include "Coordinate.h"
#include "core/pieces/Pawn.h"
#include "core/pieces/Piece.h"
#include "core/pieces/logic/PieceColorAndType.h"
#include "core/pieces/logic/PieceFinder.h"

#include <memory>

Chess::Coordinate Chess::PieceTakeLocator::Find(const std::shared_ptr<Piece>& piece, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard, Coordinate to)
{
    auto       pawnOnPassCoordinate = Coordinate(to.GetFile(), piece->GetPosition().GetRank());
    const auto finder               = std::make_shared<PieceFinder>(piecesOnBoard);
    const auto currentPawn          = std::dynamic_pointer_cast<Pawn>(piece);
    const auto opponentPawn         = std::dynamic_pointer_cast<Pawn>(finder->Find(pawnOnPassCoordinate));

    if (currentPawn && opponentPawn && opponentPawn->GetCanEnPassant()
        && opponentPawn->GetColorAndType().GetColor() != currentPawn->GetColorAndType().GetColor())
    {
        return pawnOnPassCoordinate;
    }
    return to;
}
