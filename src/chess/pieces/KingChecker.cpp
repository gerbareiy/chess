#include "KingChecker.h"

#include "ePieceType.h"
#include "../Counts.h"
#include "../Sizes.h"
#include "../eError.h"
#include "../ErrorConverter.h"

#include <algorithm>
#include <stdexcept>

std::vector<Chess::Coordinate> Chess::KingChecker::FilterMoves(std::vector<Coordinate> moves, const std::shared_ptr<King> piece, const std::vector<std::shared_ptr<IPiece>> piecesOnBoard)
{
	//std::vector<Chess::Coordinate> filteredMoves;
	//filteredMoves.reserve(COUNT_OF_KING_WAYS);

	//for (const auto& move : moves)
	//{
	//	auto pieceIterator = std::find_if(piecesOnBoard.begin(), piecesOnBoard.end(), [move, piece](const std::shared_ptr<IPiece> piece)
	//			{
	//				return  piece->get_Position() == move && piece->get_ColorAndType().get_Color() == piece->get_ColorAndType().get_Color();
	//			});

	//	if (pieceIterator == piecesOnBoard.end())
	//	{
	//		filteredMoves.push_back(move);
	//	}
	//}

	return std::vector<Coordinate>();
}

std::vector<Chess::Coordinate> Chess::KingChecker::GetAllMoves(const std::shared_ptr<King> piece, const std::vector<std::shared_ptr<IPiece>> piecesOnBoard)
{
	if (piece->get_Position().get_File() < 'A'
		|| piece->get_Position().get_File() > 'A' + CHESSBOARD_SIZE - 1
		|| piece->get_Position().get_Rank() < 1
		|| piece->get_Position().get_Rank() > CHESSBOARD_SIZE)
	{
		throw std::out_of_range(ErrorConverter::ToString(Chess::eError::OUT_OF_CHESSBOARD));
	}

	std::vector<Coordinate> moves;
	moves.reserve(COUNT_OF_KING_WAYS);

	//for (auto deltaFile = -1; deltaFile <= 1; ++deltaFile)
	//{
	//	for (auto deltaRank = -1; deltaRank <= 1; ++deltaRank)
	//	{
	//		if (deltaFile == 0 && deltaRank == 0)
	//		{
	//			continue;
	//		}

	//		auto newFile = piece->get_Position().get_File() + deltaFile;
	//		auto newRank = piece->get_Position().get_Rank() + deltaRank;

	//		if (newFile >= 'A' && newFile <= 'A' + CHESSBOARD_SIZE - 1 && newRank >= 1 && newRank <= CHESSBOARD_SIZE)
	//		{
	//			moves.push_back(Coordinate(newFile, newRank));
	//		}
	//	}
	//}

	return moves;
}

std::vector<Chess::Coordinate> Chess::KingChecker::GetPossibleMoves(const std::shared_ptr<IPiece> piece, const std::vector<std::shared_ptr<IPiece>> piecesOnBoard)
{
	if (!piece || typeid(piece) != typeid(King) || piece->get_ColorAndType().get_Type() != ePieceType::KING)
	{
		throw std::out_of_range(ErrorConverter::ToString(eError::NOT_CORRECT_PIECE));
	}

	auto allMoves = GetAllMoves(std::static_pointer_cast<King>(piece), piecesOnBoard);

	return FilterMoves(allMoves, std::static_pointer_cast<King>(piece), piecesOnBoard);
}
