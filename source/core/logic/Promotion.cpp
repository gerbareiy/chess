#include "Promotion.h"

#include "Coordinate.h"
#include "PromotePieceInputer.h"
#include "Sizes.h"
#include "core/pieces/Bishop.h"
#include "core/pieces/Knight.h"
#include "core/pieces/Pawn.h"
#include "core/pieces/Queen.h"
#include "core/pieces/Rook.h"
#include "core/pieces/logic/PieceFinder.h"
#include "core/pieces/logic/ePieceColor.h"

void Chess::Promotion::PromoteConditionally(const std::shared_ptr<Pawn>& pawn, std::vector<std::shared_ptr<Piece>>& piecesOnBoard)
{
    if (!pawn)
    {
        return;
    }

    if (pawn->GetPosition().rank == 1 && pawn->GetColorAndType().GetColor() == ePieceColor::BLACK
        || pawn->GetPosition().rank == CHESSBOARD_SIZE && pawn->GetColorAndType().GetColor() == ePieceColor::WHITE)
    {
        const auto             promoteType = PromotePieceInputer().Input();
        std::shared_ptr<Piece> piece;
        auto                   color    = pawn->GetColorAndType().GetColor();
        auto                   position = pawn->GetPosition();

        switch (promoteType)
        {
        case ePieceType::BISHOP:
            piece = std::make_shared<Bishop>(color, position);
            break;
        case ePieceType::KNIGHT:
            piece = std::make_shared<Knight>(color, position);
            break;
        case ePieceType::QUEEN:
            piece = std::make_shared<Queen>(color, position);
            break;
        case ePieceType::ROOK:
            piece = std::make_shared<Rook>(color, position);
            break;
        default:
            break;
        }

        if (!piece)
        {
            return;
        }
        piecesOnBoard.emplace_back(piece);
        piecesOnBoard.erase(std::ranges::find(piecesOnBoard, pawn));
    }
}

void Chess::Promotion::PromoteConditionally(const Coordinate& pawnPosition, std::vector<std::shared_ptr<Piece>>& piecesOnBoard)
{
    const auto finder = std::make_unique<PieceFinder>(piecesOnBoard);
    PromoteConditionally(std::dynamic_pointer_cast<Pawn>(finder->Find(pawnPosition)), piecesOnBoard);
}
