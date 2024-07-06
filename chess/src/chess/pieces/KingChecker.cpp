#include "KingChecker.h"

#include "logic/ePieceType.h"
#include "logic/ICastlable.h"
#include "../logic/Counts.h"
#include "../logic/eError.h"
#include "../logic/ErrorConverter.h"
#include "../logic/Sizes.h"

#include <algorithm>
#include <stdexcept>

std::vector<Chess::Coordinate> Chess::KingChecker::FindCastlingMoves(const std::shared_ptr<King>& king, const std::shared_ptr<PieceFinder>& finder) const
{
	std::vector<Coordinate> castlingMoves;

	if (!king->get_IsCheck() && king->get_CanMakeCastling())
	{
		auto leftRook = finder->Find(Coordinate('A', king->get_Position().get_Rank()));
		auto rightRook = finder->Find(Coordinate('A' + CHESSBOARD_SIZE - 1, king->get_Position().get_Rank()));

		auto canCastleLeft = leftRook && std::dynamic_pointer_cast<ICastable>(leftRook)
			&& std::dynamic_pointer_cast<ICastable>(leftRook)->get_CanMakeCastling()
			&& leftRook->get_ColorAndType().get_Color() == king->get_ColorAndType().get_Color();

		auto canCastleRight = rightRook && std::dynamic_pointer_cast<ICastable>(rightRook)
			&& std::dynamic_pointer_cast<ICastable>(rightRook)->get_CanMakeCastling()
			&& rightRook->get_ColorAndType().get_Color() == king->get_ColorAndType().get_Color();

		if (canCastleLeft)
		{
			auto pathClear = true;
			for (char file = 'B'; file < king->get_Position().get_File(); ++file)
			{
				if (finder->Find(Coordinate(file, king->get_Position().get_Rank())))
				{
					pathClear = false;
					break;
				}
			}

			if (pathClear)
			{
				castlingMoves.emplace_back(king->get_Position().get_File() - 2, king->get_Position().get_Rank());
			}
		}

		if (canCastleRight)
		{
			auto pathClear = true;
			for (char file = king->get_Position().get_File() + 1; file < 'A' + CHESSBOARD_SIZE - 1; ++file)
			{
				if (finder->Find(Coordinate(file, king->get_Position().get_Rank())))
				{
					pathClear = false;
					break;
				}
			}

			if (pathClear)
			{
				castlingMoves.emplace_back(king->get_Position().get_File() + 2, king->get_Position().get_Rank());
			}
		}
	}

	return castlingMoves;
}

std::vector<Chess::Coordinate> Chess::KingChecker::FindPossibleMoves(const std::shared_ptr<King>& king, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const
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

	const auto finder = std::make_shared<PieceFinder>(piecesOnBoard);

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

			auto piece = finder->Find(Coordinate(newFile, newRank));

			if (newFile >= 'A' && newFile <= 'A' + CHESSBOARD_SIZE - 1 && newRank >= 1 && newRank <= CHESSBOARD_SIZE
				&& (!piece || piece->get_ColorAndType().get_Color() != king->get_ColorAndType().get_Color()))
			{
				moves.emplace_back(newFile, newRank);
			}
		}
	}

	auto castlingMoves = FindCastlingMoves(king, finder);
	moves.insert(moves.end(), castlingMoves.begin(), castlingMoves.end());

	return moves;
}

std::vector<Chess::Coordinate> Chess::KingChecker::GetMoves(const std::shared_ptr<Piece>& piece, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const
{
	if (!piece || typeid(*piece) != typeid(King) || piece->get_ColorAndType().get_Type() != ePieceType::KING)
	{
		return std::vector<Coordinate>();
	}

	return FindPossibleMoves(std::static_pointer_cast<King>(piece), piecesOnBoard);
}
