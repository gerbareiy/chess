#include "KingChecker.h"

#include "ePieceType.h"
#include "../Counts.h"
#include "../eError.h"
#include "../ErrorConverter.h"
#include "../Sizes.h"

#include <algorithm>
#include <stdexcept>

std::vector<Chess::Coordinate> Chess::KingChecker::FilterMoves(std::vector<Coordinate> moves, const std::shared_ptr<King> king, const std::vector<std::shared_ptr<IPiece>> piecesOnBoard)
{
	//std::vector<Chess::Coordinate> filteredMoves;
	//filteredMoves.reserve(COUNT_OF_KING_WAYS);

	//for (const auto& move : moves)
	//{
	//	auto pieceIterator = std::find_if(piecesOnBoard.begin(), piecesOnBoard.end(), [move, king](const std::shared_ptr<IPiece> piece)
	//		{
	//			return piece->get_Position() == move && piece->get_ColorAndType().get_Color() == king->get_ColorAndType().get_Color();

	//			//TODO: Remake this logic -> maybe bug
	//		});

	//	if (pieceIterator == piecesOnBoard.end())
	//	{
	//		filteredMoves.push_back(move);
	//	}
	//}

	return std::vector<Coordinate>();
}

std::vector<Chess::Coordinate> Chess::KingChecker::FindPossibleMoves(const std::shared_ptr<King> king)
{
	if (king->get_Position().get_File() < 'A'
		|| king->get_Position().get_File() > 'A' + CHESSBOARD_SIZE - 1
		|| king->get_Position().get_Rank() < 1
		|| king->get_Position().get_Rank() > CHESSBOARD_SIZE)
	{
		throw std::out_of_range(ErrorConverter::ToString(Chess::eError::OUT_OF_CHESSBOARD));
	}

	std::vector<Coordinate> moves;
	moves.reserve(COUNT_OF_KING_WAYS);

	for (auto deltaFile = -1; deltaFile <= 1; ++deltaFile)
	{
		for (auto deltaRank = -1; deltaRank <= 1; ++deltaRank)
		{
			if (deltaFile == 0 && deltaRank == 0)
			{
				continue;
			}

			auto newFile = king->get_Position().get_File() + deltaFile;
			auto newRank = king->get_Position().get_Rank() + deltaRank;

			if (newFile >= 'A' && newFile <= 'A' + CHESSBOARD_SIZE - 1 && newRank >= 1 && newRank <= CHESSBOARD_SIZE)
			{
				moves.push_back(Coordinate(newFile, newRank));
			}
		}
	}

	//TODO:: Add a castling logic

	return moves;
}

std::vector<Chess::Coordinate> Chess::KingChecker::GetPossibleMoves(const std::shared_ptr<IPiece> piece, const std::vector<std::shared_ptr<IPiece>> piecesOnBoard)
{
	if (!piece || typeid(piece) != typeid(King) || piece->get_ColorAndType().get_Type() != ePieceType::KING)
	{
		throw std::out_of_range(ErrorConverter::ToString(eError::NOT_CORRECT_PIECE));
	}

	auto allMoves = FindPossibleMoves(std::static_pointer_cast<King>(piece));

	return FilterMoves(allMoves, std::static_pointer_cast<King>(piece), piecesOnBoard);
}
