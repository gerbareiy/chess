#include "PieceTypeConverter.h"

std::string Chess::PieceTypeConverter::ConvertToString(ePieceType type)
{
	switch (type)
	{
	case ePieceType::NONE:
		return " ";
	case ePieceType::BISHOP:
		return "B";
	case ePieceType::KING:
		return "K";
	case ePieceType::KNIGHT:
		return "k";
	case ePieceType::PAWN:
		return "P";
	case ePieceType::QUEEN:
		return "Q";
	case ePieceType::ROOK:
		return "R";
	default:
		return "Unknown piece";
	}
}
