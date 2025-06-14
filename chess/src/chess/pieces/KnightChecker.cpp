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

std::vector<Chess::Coordinate> Chess::KnightChecker::FindPossibleMoves(std::shared_ptr<Knight> const& knight, std::vector<std::shared_ptr<Piece>> const& piecesOnBoard) const
{
	if (knight->GetPosition().GetFile() < 'A'
		|| knight->GetPosition().GetFile() >= 'A' + CHESSBOARD_SIZE
		|| knight->GetPosition().GetRank() < 1
		|| knight->GetPosition().GetRank() > CHESSBOARD_SIZE)
	{
		throw std::out_of_range(ErrorConverter::ToString(Chess::eError::OUT_OF_CHESSBOARD));
	}

	std::vector<Coordinate> moves;
	moves.reserve(COUNT_OF_KNIGHT_WAYS);

	auto currentPos = knight->GetPosition();
	auto finder = std::make_shared<PieceFinder>(piecesOnBoard);

	for (auto const& move : m_knightMoveDirections)
	{
		auto newFile = currentPos.GetFile() + move.first;
		auto newRank = currentPos.GetRank() + move.second;

		if (newFile < 'A' || newFile >= 'A' + CHESSBOARD_SIZE || newRank < 1 || newRank > CHESSBOARD_SIZE)
		{
			continue;
		}

		auto piece = finder->Find(Coordinate(newFile, newRank));

		if (!piece || piece->GetColorAndType().GetColor() != knight->GetColorAndType().GetColor())
		{
			moves.emplace_back(newFile, newRank);
		}
	}

	return moves;
}

std::vector<Chess::Coordinate> Chess::KnightChecker::GetMoves(std::shared_ptr<Piece> const& piece, std::vector<std::shared_ptr<Piece>> const& piecesOnBoard) const
{
	if (!piece || typeid(*piece) != typeid(Knight) || piece->GetColorAndType().GetType() != ePieceType::KNIGHT)
	{
		return std::vector<Coordinate>();
	}

	return FindPossibleMoves(std::static_pointer_cast<Knight>(piece), piecesOnBoard);
}
