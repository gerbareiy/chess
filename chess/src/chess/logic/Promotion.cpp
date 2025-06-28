#include "Promotion.h"

#include "../LabelShower.h"
#include "../pieces/Bishop.h"
#include "../pieces/Knight.h"
#include "../pieces/Pawn.h"
#include "../pieces/Piece.h"
#include "../pieces/Queen.h"
#include "../pieces/Rook.h"
#include "../pieces/logic/PieceColorAndType.h"
#include "../pieces/logic/PieceFinder.h"
#include "../pieces/logic/ePieceColor.h"
#include "../pieces/logic/ePieceType.h"
#include "Coordinate.h"
#include "PromotePieceInputer.h"
#include "Sizes.h"

void Chess::Promotion::PromoteConditionally(const std::shared_ptr<Pawn>& pawn, std::vector<std::shared_ptr<Piece>>& piecesOnBoard)
{
    if (!pawn)
    {
        return;
    }

    if (pawn->GetPosition().GetRank() == 1 && pawn->GetColorAndType().GetColor() == ePieceColor::BLACK ||
        pawn->GetPosition().GetRank() == CHESSBOARD_SIZE && pawn->GetColorAndType().GetColor() == ePieceColor::WHITE)
    {
        auto                   inputer        = std::make_shared<PromotePieceInputer>();
        auto                   inputDisplayer = std::make_unique<LabelShower>(inputer);
        const auto             promoteType    = inputer->Input();
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
