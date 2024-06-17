#include "ErrorConverter.h"

std::string Chess::ErrorConverter::ToString(eError error)
{
	switch (error)
	{
	case eError::OUT_OF_CHESSBOARD:
		return "ChessPiece is out of the Chessboard";
	case eError::OUT_OF_COUNT_OF_BISHOP_KNIGHT_ROOK_WITH_1_COLOR:
		return "Wrong order number was entered";
	case eError::NOT_CORRECT_PIECE:
		return "Piece is not correct";
	case eError::NOT_CORRECT_MOVE:
		return "Move is imposible";
	default:
		return "Unknown error";
	}
}
