#include "Promotion.h"

#include "Sizes.h"
#include "../pieces/logic/ePieceColor.h"

void Chess::Promotion::Promote(std::shared_ptr<Pawn> pawn, std::vector<std::shared_ptr<IPiece>>& piecesOnBoard)
{
	if (!pawn)
	{
		return;
	}

	if (pawn->get_Position().get_Rank() == 1 && pawn->get_ColorAndType().get_Color() == ePieceColor::BLACK
		|| pawn->get_Position().get_Rank() == CHESSBOARD_SIZE && pawn->get_ColorAndType().get_Color() == ePieceColor::WHITE)
	{

	}
}
