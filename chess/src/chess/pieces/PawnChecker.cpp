#include "PawnChecker.h"

#include "logic/ePieceColor.h"
#include "../Counts.h"
#include "../eError.h"
#include "../ErrorConverter.h"
#include "../PositionChecker.h"
#include "../Sizes.h"

#include <stdexcept>
#include <algorithm>

std::vector<Chess::Coordinate> Chess::PawnChecker::FindPossibleMoves(const std::shared_ptr<Pawn>& pawn, const std::vector<std::shared_ptr<IPiece>>& piecesOnBoard, const std::shared_ptr<PieceFinder>& finder)
{
	ValidatePawn(pawn);

	auto moveVector = (pawn->get_ColorAndType().get_Color() == ePieceColor::WHITE) ? 1 : -1;

	std::vector<Coordinate> moves;
	moves.reserve(COUNT_OF_PAWN_WAYS);

	Coordinate oneStepForward(pawn->get_Position().get_File(), pawn->get_Position().get_Rank() + moveVector);

	if (PositionChecker::IsPositionValid(oneStepForward) && !finder->Find(oneStepForward))
	{
		moves.push_back(oneStepForward);

		if (pawn->get_IsNotMoved())
		{
			Coordinate twoStepsForward(pawn->get_Position().get_File(), pawn->get_Position().get_Rank() + (moveVector << 1));

			if (PositionChecker::IsPositionValid(twoStepsForward) && !finder->Find(twoStepsForward))
			{
				moves.push_back(twoStepsForward);
			}
		}
	}

	return moves;
}

std::vector<Chess::Coordinate> Chess::PawnChecker::GetDiagonalMoves(const std::shared_ptr<Pawn>& pawn, const std::vector<std::shared_ptr<IPiece>>& piecesOnBoard, const std::shared_ptr<PieceFinder>& finder)
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
			moves.push_back(rightDiagonal);
		}
		else if (rightPiece	&& typeid(*rightPiece) == typeid(Pawn)
			&& std::static_pointer_cast<Pawn>(rightPiece)->get_CanEnPassant()
			&& rightPiece->get_ColorAndType().get_Color() != pawn->get_ColorAndType().get_Color())
		{
			moves.push_back(rightDiagonal);
		}
	}

	if (PositionChecker::IsPositionValid(leftDiagonal))
	{
		auto leftDiagonalPiece = finder->Find(leftDiagonal);
		auto leftPiece = finder->Find(left);

		if (leftDiagonalPiece && leftDiagonalPiece->get_ColorAndType().get_Color() != pawn->get_ColorAndType().get_Color())
		{
			moves.push_back(leftDiagonal);
		}
		else if (leftPiece && typeid(*leftPiece) == typeid(Pawn)
			&& std::static_pointer_cast<Pawn>(leftPiece)->get_CanEnPassant()
			&& leftPiece->get_ColorAndType().get_Color() != pawn->get_ColorAndType().get_Color())
		{
			moves.push_back(leftDiagonal);
		}
	}

	return moves;
}

void Chess::PawnChecker::ValidatePawn(const std::shared_ptr<Pawn>& pawn)
{
	if (!PositionChecker::IsPositionValid(pawn->get_Position()))
	{
		throw std::out_of_range(ErrorConverter::ToString(eError::OUT_OF_CHESSBOARD));
	}
	if (pawn->get_ColorAndType().get_Color() == ePieceColor::NONE)
	{
		throw std::out_of_range(ErrorConverter::ToString(eError::NOT_CORRECT_PIECE));
	}
}

std::vector<Chess::Coordinate> Chess::PawnChecker::GetPossibleMoves(const std::shared_ptr<IPiece>& piece, const std::vector<std::shared_ptr<IPiece>>& piecesOnBoard)
{
	if (!piece || typeid(*piece) != typeid(Pawn) || piece->get_ColorAndType().get_Type() != ePieceType::PAWN)
	{
		throw std::out_of_range(ErrorConverter::ToString(eError::NOT_CORRECT_PIECE));
	}

	std::vector<Coordinate> allMoves;

	const auto finder = std::make_shared<PieceFinder>(piecesOnBoard);
	auto forwardMoves = FindPossibleMoves(std::static_pointer_cast<Pawn>(piece), piecesOnBoard, finder);
	auto diagonalMoves = GetDiagonalMoves(std::static_pointer_cast<Pawn>(piece), piecesOnBoard, finder);

	allMoves.insert(allMoves.end(), forwardMoves.begin(), forwardMoves.end());
	allMoves.insert(allMoves.end(), diagonalMoves.begin(), diagonalMoves.end());

	return allMoves;
}
