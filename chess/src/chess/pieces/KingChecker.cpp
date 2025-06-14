#include "KingChecker.h"

#include "King.h"
#include "logic/ePieceType.h"
#include "logic/ICastlable.h"
#include "logic/PieceColorAndType.h"
#include "logic/PieceFinder.h"
#include "Piece.h"
#include "../logic/Coordinate.h"
#include "../logic/Counts.h"
#include "../logic/eError.h"
#include "../logic/ErrorConverter.h"
#include "../logic/Sizes.h"

#include <stdexcept>

std::vector<Chess::Coordinate> Chess::KingChecker::FindCastlingMoves(std::shared_ptr<King> const& king, std::shared_ptr<PieceFinder> const& finder) const
{
	std::vector<Coordinate> castlingMoves;

	if (!king->GetIsCheck() && king->GetCanMakeCastling())
	{
		auto leftRook = finder->Find(Coordinate('A', king->GetPosition().GetRank()));
		auto rightRook = finder->Find(Coordinate('A' + CHESSBOARD_SIZE - 1, king->GetPosition().GetRank()));

		auto canCastleLeft = leftRook && std::dynamic_pointer_cast<ICastable>(leftRook)
			&& std::dynamic_pointer_cast<ICastable>(leftRook)->GetCanMakeCastling()
			&& leftRook->GetColorAndType().GetColor() == king->GetColorAndType().GetColor();

		auto canCastleRight = rightRook && std::dynamic_pointer_cast<ICastable>(rightRook)
			&& std::dynamic_pointer_cast<ICastable>(rightRook)->GetCanMakeCastling()
			&& rightRook->GetColorAndType().GetColor() == king->GetColorAndType().GetColor();

		if (canCastleLeft)
		{
			auto pathClear = true;
			for (char file = 'B'; file < king->GetPosition().GetFile(); ++file)
			{
				if (finder->Find(Coordinate(file, king->GetPosition().GetRank())))
				{
					pathClear = false;
					break;
				}
			}

			if (pathClear)
			{
				castlingMoves.emplace_back(king->GetPosition().GetFile() - 2, king->GetPosition().GetRank());
			}
		}

		if (canCastleRight)
		{
			auto pathClear = true;
			for (char file = king->GetPosition().GetFile() + 1; file < 'A' + CHESSBOARD_SIZE - 1; ++file)
			{
				if (finder->Find(Coordinate(file, king->GetPosition().GetRank())))
				{
					pathClear = false;
					break;
				}
			}

			if (pathClear)
			{
				castlingMoves.emplace_back(king->GetPosition().GetFile() + 2, king->GetPosition().GetRank());
			}
		}
	}

	return castlingMoves;
}

std::vector<Chess::Coordinate> Chess::KingChecker::FindPossibleMoves(std::shared_ptr<King> const& king, std::vector<std::shared_ptr<Piece>> const& piecesOnBoard) const
{
	if (king->GetPosition().GetFile() < 'A'
		|| king->GetPosition().GetFile() > 'A' + CHESSBOARD_SIZE - 1
		|| king->GetPosition().GetRank() < 1
		|| king->GetPosition().GetRank() > CHESSBOARD_SIZE)
	{
		throw std::out_of_range(ErrorConverter::ToString(Chess::eError::OUT_OF_CHESSBOARD));
	}

	std::vector<Coordinate> moves;
	moves.reserve(COUNT_OF_KING_WAYS);

	auto const finder = std::make_shared<PieceFinder>(piecesOnBoard);

	for (auto deltaFile = -1; deltaFile <= 1; ++deltaFile)
	{
		for (auto deltaRank = -1; deltaRank <= 1; ++deltaRank)
		{
			if (deltaFile == 0 && deltaRank == 0)
			{
				continue;
			}

			auto newFile = king->GetPosition().GetFile() + deltaFile;
			auto newRank = king->GetPosition().GetRank() + deltaRank;

			auto piece = finder->Find(Coordinate(newFile, newRank));

			if (newFile >= 'A' && newFile <= 'A' + CHESSBOARD_SIZE - 1 && newRank >= 1 && newRank <= CHESSBOARD_SIZE
				&& (!piece || piece->GetColorAndType().GetColor() != king->GetColorAndType().GetColor()))
			{
				moves.emplace_back(newFile, newRank);
			}
		}
	}

	auto castlingMoves = FindCastlingMoves(king, finder);
	moves.insert(moves.end(), castlingMoves.begin(), castlingMoves.end());

	return moves;
}

std::vector<Chess::Coordinate> Chess::KingChecker::GetMoves(std::shared_ptr<Piece> const& piece, std::vector<std::shared_ptr<Piece>> const& piecesOnBoard) const
{
	if (!piece || typeid(*piece) != typeid(King) || piece->GetColorAndType().GetType() != ePieceType::KING)
	{
		return std::vector<Coordinate>();
	}

	return FindPossibleMoves(std::static_pointer_cast<King>(piece), piecesOnBoard);
}
