#include "PawnChecker.h"

#include "logic/ePieceColor.h"
#include "../logic/Counts.h"
#include "../logic/eError.h"
#include "../logic/ErrorConverter.h"
#include "logic/PositionChecker.h"
#include "../logic/Sizes.h"

#include <stdexcept>
#include <algorithm>

std::vector<Chess::Coordinate> Chess::PawnChecker::GetForwardMoves(const std::shared_ptr<Pawn>& pawn, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard, const std::shared_ptr<PieceFinder>& finder) const
{
	ValidatePawn(pawn);

	auto moveVector = (pawn->get_ColorAndType().get_Color() == ePieceColor::WHITE) ? 1 : -1;

	std::vector<Coordinate> moves;
	moves.reserve(COUNT_OF_PAWN_WAYS);

	Coordinate oneStepForward(pawn->get_Position().get_File(), pawn->get_Position().get_Rank() + moveVector);

	if (PositionChecker::IsPositionValid(oneStepForward) && !finder->Find(oneStepForward))
	{
		moves.emplace_back(oneStepForward);

		if (pawn->get_IsNotMoved())
		{
			Coordinate twoStepsForward(pawn->get_Position().get_File(), pawn->get_Position().get_Rank() + (moveVector << 1));

			if (PositionChecker::IsPositionValid(twoStepsForward) && !finder->Find(twoStepsForward))
			{
				moves.emplace_back(twoStepsForward);
			}
		}
	}

	return moves;
}

std::vector<Chess::Coordinate> Chess::PawnChecker::GetDiagonalMoves(const std::shared_ptr<Pawn>& pawn, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard, const std::shared_ptr<PieceFinder>& finder) const
{
	ValidatePawn(pawn);

	int moveVector = (pawn->get_ColorAndType().get_Color() == ePieceColor::WHITE) ? 1 : -1;

	std::vector<Coordinate> moves;
	moves.reserve(COUNT_OF_PAWN_WAYS);

	Coordinate rightDiagonal(pawn->get_Position().get_File() + 1, pawn->get_Position().get_Rank() + moveVector);
	Coordinate leftDiagonal(pawn->get_Position().get_File() - 1, pawn->get_Position().get_Rank() + moveVector);
	Coordinate left(pawn->get_Position().get_File() - 1, pawn->get_Position().get_Rank());
	Coordinate right(pawn->get_Position().get_File() + 1, pawn->get_Position().get_Rank());

	if (PositionChecker::IsPositionValid(rightDiagonal))
	{
		auto rightDiagonalPiece = finder->Find(rightDiagonal);
		auto rightPiece = finder->Find(right);

		if (rightDiagonalPiece && rightDiagonalPiece->get_ColorAndType().get_Color() != pawn->get_ColorAndType().get_Color())
		{
			moves.emplace_back(rightDiagonal);
		}
		else if (rightPiece && typeid(*rightPiece) == typeid(Pawn)
			&& std::static_pointer_cast<Pawn>(rightPiece)->get_CanEnPassant()
			&& rightPiece->get_ColorAndType().get_Color() != pawn->get_ColorAndType().get_Color())
		{
			moves.emplace_back(rightDiagonal);
		}
	}

	if (PositionChecker::IsPositionValid(leftDiagonal))
	{
		auto leftDiagonalPiece = finder->Find(leftDiagonal);
		auto leftPiece = finder->Find(left);

		if (leftDiagonalPiece && leftDiagonalPiece->get_ColorAndType().get_Color() != pawn->get_ColorAndType().get_Color())
		{
			moves.emplace_back(leftDiagonal);
		}
		else if (leftPiece && typeid(*leftPiece) == typeid(Pawn)
			&& std::static_pointer_cast<Pawn>(leftPiece)->get_CanEnPassant()
			&& leftPiece->get_ColorAndType().get_Color() != pawn->get_ColorAndType().get_Color())
		{
			moves.emplace_back(leftDiagonal);
		}
	}

	return moves;
}

void Chess::PawnChecker::ValidatePawn(const std::shared_ptr<Pawn>& pawn) const
{
	if (!PositionChecker::IsPositionValid(pawn->get_Position()))
	{
		throw std::out_of_range(ErrorConverter::ToString(eError::OUT_OF_CHESSBOARD));
	}
	if (pawn->get_ColorAndType().get_Color() == ePieceColor::NONE)
	{
		throw std::invalid_argument(ErrorConverter::ToString(eError::NOT_CORRECT_PIECE));
	}
}

std::vector<Chess::Coordinate> Chess::PawnChecker::GetMoves(const std::shared_ptr<Piece>& piece, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const
{
	if (!piece || typeid(*piece) != typeid(Pawn) || piece->get_ColorAndType().get_Type() != ePieceType::PAWN)
	{
		return std::vector<Coordinate>();
	}

	std::vector<Coordinate> allMoves;

	const auto finder = std::make_shared<PieceFinder>(piecesOnBoard);
	auto forwardMoves = GetForwardMoves(std::static_pointer_cast<Pawn>(piece), piecesOnBoard, finder);
	auto diagonalMoves = GetDiagonalMoves(std::static_pointer_cast<Pawn>(piece), piecesOnBoard, finder);

	allMoves.insert(allMoves.end(), forwardMoves.begin(), forwardMoves.end());
	allMoves.insert(allMoves.end(), diagonalMoves.begin(), diagonalMoves.end());

	return allMoves;
}
