#include "Promotion.h"

#include "Coordinate.h"
#include "PromotePieceInputer.h"
#include "Sizes.h"
#include "../LableDisplayer.h"
#include "../pieces/Bishop.h"
#include "../pieces/Knight.h"
#include "../pieces/Queen.h"
#include "../pieces/Rook.h"
#include "../pieces/logic/ePieceColor.h"
#include "../pieces/logic/ePieceType.h"
#include "../pieces/logic/PieceColorAndType.h"
#include "../pieces/logic/PieceFinder.h"
#include "../pieces/Pawn.h"
#include "../pieces/Piece.h"

void Chess::Promotion::PromoteConditionally(std::shared_ptr<Pawn> pawn, std::vector<std::shared_ptr<Piece>>& piecesOnBoard)
{
	if (!pawn)
	{
		return;
	}

	if (pawn->GetPosition().GetRank() == 1 && pawn->GetColorAndType().GetColor() == ePieceColor::BLACK
		|| pawn->GetPosition().GetRank() == CHESSBOARD_SIZE && pawn->GetColorAndType().GetColor() == ePieceColor::WHITE)
	{
		auto inputer = std::make_shared<PromotePieceInputer>();
		auto inputDisplayer = std::make_unique<LableDisplayer>(inputer);
		auto promoteType = inputer->Input();
		std::shared_ptr<Piece> piece;
		auto color = pawn->GetColorAndType().GetColor();
		auto position = pawn->GetPosition();

		switch (promoteType)
		{
		case Chess::ePieceType::BISHOP:
			piece = std::make_shared<Bishop>(color, position);
			break;
		case Chess::ePieceType::KNIGHT:
			piece = std::make_shared<Knight>(color, position);
			break;
		case Chess::ePieceType::QUEEN:
			piece = std::make_shared<Queen>(color, position);
			break;
		case Chess::ePieceType::ROOK:
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
		piecesOnBoard.erase(std::find(piecesOnBoard.begin(), piecesOnBoard.end(), pawn));
	}
}

void Chess::Promotion::PromoteConditionally(Coordinate pawnPosition, std::vector<std::shared_ptr<Piece>>& piecesOnBoard)
{
	auto finder = std::make_unique<PieceFinder>(piecesOnBoard);
	PromoteConditionally(std::dynamic_pointer_cast<Pawn>(finder->Find(pawnPosition)), piecesOnBoard);
}
