#include "KnightChecker.h"

#include "../Sizes.h"
#include "../Counts.h"
#include "../eError.h"
#include "../ErrorConverter.h"

#include <stdexcept>

std::vector<Chess::Coordinate> Chess::KnightChecker::FilterMoves(const std::vector<Coordinate> moves, const std::shared_ptr<Knight> piece, const std::vector<std::shared_ptr<IPiece>> piecesOnBoard)
{
	std::vector<Chess::Coordinate> filteredMoves;
	filteredMoves.reserve(COUNT_OF_KNIGHT_WAYS);

	for (const auto& move : moves)
	{
		auto pieceIterator = std::find_if(piecesOnBoard.begin(), piecesOnBoard.end(), [move, piece](const std::shared_ptr<IPiece> condidate)
			{
				return condidate->get_Position() == move && condidate->get_ColorAndType().get_Color() == piece->get_ColorAndType().get_Color();
			});

		if (pieceIterator == piecesOnBoard.end())
		{
			filteredMoves.push_back(move);
		}
	}

	return filteredMoves;
}

std::vector<Chess::Coordinate> Chess::KnightChecker::GetAllMoves(const std::shared_ptr<Knight> piece, const std::vector<std::shared_ptr<IPiece>> piecesOnBoard)
{
	if (piece->get_Position().get_File() < 'A'
		|| piece->get_Position().get_File() > 'A' + CHESSBOARD_SIZE - 1
		|| piece->get_Position().get_Rank() < 1
		|| piece->get_Position().get_Rank() > CHESSBOARD_SIZE)
	{
		throw std::out_of_range(ErrorConverter::ToString(Chess::eError::OUT_OF_CHESSBOARD));
	}

	std::vector<Coordinate> moves;
	moves.reserve(COUNT_OF_KNIGHT_WAYS);

	auto currentPos = piece->get_Position();

	for (const auto& move : m_knightMoves)
	{
		auto newFile = currentPos.get_File() + move.first;
		auto newRank = currentPos.get_Rank() + move.second;

		if (newFile < 'A' || newFile >= 'A' + CHESSBOARD_SIZE || newRank < 1 || newRank > CHESSBOARD_SIZE)
		{
			continue;
		}

		Coordinate newPos(newFile, newRank);

		auto isOccupied = false;

		for (const auto& p : piecesOnBoard)
		{
			if (p->get_Position() == newPos)
			{
				isOccupied = true;
				break;
			}
		}

		if (!isOccupied)
		{
			moves.push_back(newPos);
		}
	}

	return moves;
}

std::vector<Chess::Coordinate> Chess::KnightChecker::GetPossibleMoves(const std::shared_ptr<IPiece> piece, const std::vector<std::shared_ptr<IPiece>> piecesOnBoard)
{
	if (!piece || typeid(piece) != typeid(Knight) || piece->get_ColorAndType().get_Type() != ePieceType::KNIGHT)
	{
		throw std::out_of_range(ErrorConverter::ToString(eError::NOT_CORRECT_PIECE));
	}

	auto allMoves = GetAllMoves(std::static_pointer_cast<Knight>(piece), piecesOnBoard);

	return FilterMoves(allMoves, std::static_pointer_cast<Knight>(piece), piecesOnBoard);
}
