#include "KnightChecker.h"

#include "Knight.h"
#include "logic/ePieceType.h"
#include "logic/PieceColorAndType.h"
#include "logic/PieceFinder.h"
#include "Piece.h"
#include "../logic/Coordinate.h"
#include "../logic/Counts.h"
#include "../logic/eError.h"
#include "../logic/ErrorConverter.h"
#include "../logic/Sizes.h"

#include <memory>
#include <stdexcept>

std::vector<Chess::Coordinate> Chess::KnightChecker::FindPossibleMoves(const std::shared_ptr<Knight>& knight, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const
{
	if (knight->get_Position().get_File() < 'A'
		|| knight->get_Position().get_File() >= 'A' + CHESSBOARD_SIZE
		|| knight->get_Position().get_Rank() < 1
		|| knight->get_Position().get_Rank() > CHESSBOARD_SIZE)
	{
		throw std::out_of_range(ErrorConverter::ToString(Chess::eError::OUT_OF_CHESSBOARD));
	}

	std::vector<Coordinate> moves;
	moves.reserve(COUNT_OF_KNIGHT_WAYS);

	auto currentPos = knight->get_Position();
	auto finder = std::make_shared<PieceFinder>(piecesOnBoard);

	for (const auto& move : m_knightMoveDirections)
	{
		auto newFile = currentPos.get_File() + move.first;
		auto newRank = currentPos.get_Rank() + move.second;

		if (newFile < 'A' || newFile >= 'A' + CHESSBOARD_SIZE || newRank < 1 || newRank > CHESSBOARD_SIZE)
		{
			continue;
		}

		auto piece = finder->Find(Coordinate(newFile, newRank));

		if (!piece || piece->get_ColorAndType().get_Color() != knight->get_ColorAndType().get_Color())
		{
			moves.emplace_back(newFile, newRank);
		}
	}

	return moves;
}

std::vector<Chess::Coordinate> Chess::KnightChecker::GetMoves(const std::shared_ptr<Piece>& piece, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const
{
	if (!piece || typeid(*piece) != typeid(Knight) || piece->get_ColorAndType().get_Type() != ePieceType::KNIGHT)
	{
		return std::vector<Coordinate>();
	}

	return FindPossibleMoves(std::static_pointer_cast<Knight>(piece), piecesOnBoard);
}
