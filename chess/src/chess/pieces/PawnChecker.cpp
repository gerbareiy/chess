#include "PawnChecker.h"

#include "logic/ePieceColor.h"
#include "logic/ePieceType.h"
#include "logic/PieceColorAndType.h"
#include "logic/PieceFinder.h"
#include "logic/PositionChecker.h"
#include "Pawn.h"
#include "Piece.h"
#include "../logic/Coordinate.h"
#include "../logic/Counts.h"
#include "../logic/eError.h"
#include "../logic/ErrorConverter.h"

#include <stdexcept>

std::vector<Chess::Coordinate> Chess::PawnChecker::GetForwardMoves(std::shared_ptr<Pawn> const& pawn, std::vector<std::shared_ptr<Piece>> const& piecesOnBoard, std::shared_ptr<PieceFinder> const& finder) const
{
	ValidatePawn(pawn);

	auto moveVector = (pawn->GetColorAndType().GetColor() == ePieceColor::WHITE) ? 1 : -1;

	std::vector<Coordinate> moves;
	moves.reserve(COUNT_OF_PAWN_WAYS);

	Coordinate oneStepForward(pawn->GetPosition().GetFile(), pawn->GetPosition().GetRank() + moveVector);

	if (PositionChecker::IsPositionValid(oneStepForward) && !finder->Find(oneStepForward))
	{
		moves.emplace_back(oneStepForward);

		if (pawn->GetIsNotMoved())
		{
			Coordinate twoStepsForward(pawn->GetPosition().GetFile(), pawn->GetPosition().GetRank() + (moveVector << 1));

			if (PositionChecker::IsPositionValid(twoStepsForward) && !finder->Find(twoStepsForward))
			{
				moves.emplace_back(twoStepsForward);
			}
		}
	}

	return moves;
}

std::vector<Chess::Coordinate> Chess::PawnChecker::GetDiagonalMoves(std::shared_ptr<Pawn> const& pawn, std::vector<std::shared_ptr<Piece>> const& piecesOnBoard, std::shared_ptr<PieceFinder> const& finder) const
{
	ValidatePawn(pawn);

	int moveVector = (pawn->GetColorAndType().GetColor() == ePieceColor::WHITE) ? 1 : -1;

	std::vector<Coordinate> moves;
	moves.reserve(COUNT_OF_PAWN_WAYS);

	Coordinate rightDiagonal(pawn->GetPosition().GetFile() + 1, pawn->GetPosition().GetRank() + moveVector);
	Coordinate leftDiagonal(pawn->GetPosition().GetFile() - 1, pawn->GetPosition().GetRank() + moveVector);
	Coordinate left(pawn->GetPosition().GetFile() - 1, pawn->GetPosition().GetRank());
	Coordinate right(pawn->GetPosition().GetFile() + 1, pawn->GetPosition().GetRank());

	if (PositionChecker::IsPositionValid(rightDiagonal))
	{
		auto rightDiagonalPiece = finder->Find(rightDiagonal);
		auto rightPiece = finder->Find(right);

		if (rightDiagonalPiece && rightDiagonalPiece->GetColorAndType().GetColor() != pawn->GetColorAndType().GetColor())
		{
			moves.emplace_back(rightDiagonal);
		}
		else if (rightPiece && typeid(*rightPiece) == typeid(Pawn)
			&& std::static_pointer_cast<Pawn>(rightPiece)->GetCanEnPassant()
			&& rightPiece->GetColorAndType().GetColor() != pawn->GetColorAndType().GetColor())
		{
			moves.emplace_back(rightDiagonal);
		}
	}

	if (PositionChecker::IsPositionValid(leftDiagonal))
	{
		auto leftDiagonalPiece = finder->Find(leftDiagonal);
		auto leftPiece = finder->Find(left);

		if (leftDiagonalPiece && leftDiagonalPiece->GetColorAndType().GetColor() != pawn->GetColorAndType().GetColor())
		{
			moves.emplace_back(leftDiagonal);
		}
		else if (leftPiece && typeid(*leftPiece) == typeid(Pawn)
			&& std::static_pointer_cast<Pawn>(leftPiece)->GetCanEnPassant()
			&& leftPiece->GetColorAndType().GetColor() != pawn->GetColorAndType().GetColor())
		{
			moves.emplace_back(leftDiagonal);
		}
	}

	return moves;
}

void Chess::PawnChecker::ValidatePawn(std::shared_ptr<Pawn> const& pawn) const
{
	if (!PositionChecker::IsPositionValid(pawn->GetPosition()))
	{
		throw std::out_of_range(ErrorConverter::ToString(eError::OUT_OF_CHESSBOARD));
	}
	if (pawn->GetColorAndType().GetColor() == ePieceColor::NONE)
	{
		throw std::invalid_argument(ErrorConverter::ToString(eError::NOT_CORRECT_PIECE));
	}
}

std::vector<Chess::Coordinate> Chess::PawnChecker::GetMoves(std::shared_ptr<Piece> const& piece, std::vector<std::shared_ptr<Piece>> const& piecesOnBoard) const
{
	if (!piece || typeid(*piece) != typeid(Pawn) || piece->GetColorAndType().GetType() != ePieceType::PAWN)
	{
		return std::vector<Coordinate>();
	}

	std::vector<Coordinate> allMoves;

	auto const finder = std::make_shared<PieceFinder>(piecesOnBoard);
	auto forwardMoves = GetForwardMoves(std::static_pointer_cast<Pawn>(piece), piecesOnBoard, finder);
	auto diagonalMoves = GetDiagonalMoves(std::static_pointer_cast<Pawn>(piece), piecesOnBoard, finder);

	allMoves.insert(allMoves.end(), forwardMoves.begin(), forwardMoves.end());
	allMoves.insert(allMoves.end(), diagonalMoves.begin(), diagonalMoves.end());

	return allMoves;
}
