#include "Promotion.h"

#include "PromotePieceInputer.h"
#include "Sizes.h"
#include "../LableDisplayer.h"
#include "../pieces/Bishop.h"
#include "../pieces/Knight.h"
#include "../pieces/Queen.h"
#include "../pieces/Rook.h"
#include "../pieces/logic/ePieceColor.h"
#include "../pieces/logic/ePieceType.h"
#include "../pieces/logic/PieceFinder.h"

void Chess::Promotion::PromoteConditionally(std::shared_ptr<Pawn> pawn, std::vector<std::shared_ptr<IPiece>>& piecesOnBoard)
{
	if (!pawn)
	{
		return;
	}

	if (pawn->get_Position().get_Rank() == 1 && pawn->get_ColorAndType().get_Color() == ePieceColor::BLACK
		|| pawn->get_Position().get_Rank() == CHESSBOARD_SIZE && pawn->get_ColorAndType().get_Color() == ePieceColor::WHITE)
	{
		auto inputer = std::make_shared<PromotePieceInputer>();
		auto inputDisplayer = std::make_unique<LableDisplayer>(inputer);
		auto promoteType = inputer->Input();
		std::shared_ptr<IPiece> piece;
		auto color = pawn->get_ColorAndType().get_Color();
		auto position = pawn->get_Position();

		if (promoteType == ePieceType::BISHOP)
		{
			piece = std::make_shared<Bishop>(color, position);
		}
		else if (promoteType == ePieceType::KNIGHT)
		{
			piece = std::make_shared<Knight>(color, position);
		}
		else if (promoteType == ePieceType::QUEEN)
		{
			piece = std::make_shared<Queen>(color, position);
		}
		else if (promoteType == ePieceType::ROOK)
		{
			piece = std::make_shared<Rook>(color, position);
		}
		piecesOnBoard.emplace_back(piece);
		piecesOnBoard.erase(std::find(piecesOnBoard.begin(), piecesOnBoard.end(), pawn));
	}
}

void Chess::Promotion::PromoteConditionally(Coordinate pawnPosition, std::vector<std::shared_ptr<IPiece>>& piecesOnBoard)
{
	auto finder = std::make_unique<PieceFinder>(piecesOnBoard);
	PromoteConditionally(std::dynamic_pointer_cast<Pawn>(finder->Find(pawnPosition)), piecesOnBoard);
}
