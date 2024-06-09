#include "PawnChecker.h"

#include "ePieceColor.h"
#include "../Counts.h"
#include "../eError.h"
#include "../ErrorConverter.h"
#include "../Sizes.h"

#include <stdexcept>
#include <algorithm>

std::map<Chess::Coordinate, std::shared_ptr<Chess::IPiece>> Chess::PawnChecker::CreatePieceMap(const std::vector<std::shared_ptr<IPiece>>& piecesOnBoard)
{
	std::map<Coordinate, std::shared_ptr<IPiece>> pieceMap;

	for (const auto& piece : piecesOnBoard)
	{
		pieceMap[piece->get_Position()] = piece;
	}

	return pieceMap;
}

bool Chess::PawnChecker::IsPositionValid(const Coordinate& position)
{
	return position.get_File() >= 'A'
		&& position.get_File() < 'A' + CHESSBOARD_SIZE
		&& position.get_Rank() >= 1
		&& position.get_Rank() <= CHESSBOARD_SIZE;
}

std::vector<Chess::Coordinate> Chess::PawnChecker::FindAllMoves(const std::shared_ptr<Pawn>& pawn, const std::vector<std::shared_ptr<IPiece>>& piecesOnBoard)
{
	ValidatePawn(pawn);

	auto moveVector = (pawn->get_ColorAndType().get_Color() == ePieceColor::WHITE) ? 1 : -1;

	std::vector<Coordinate> moves;
	moves.reserve(COUNT_OF_PAWN_WAYS);

	auto pieceMap = CreatePieceMap(piecesOnBoard);

	Coordinate oneStepForward(pawn->get_Position().get_File(), pawn->get_Position().get_Rank() + moveVector);

	if (IsPositionValid(oneStepForward) && pieceMap.find(oneStepForward) == pieceMap.end())
	{
		moves.push_back(oneStepForward);

		if (pawn->get_IsNotMoved())
		{
			Coordinate twoStepsForward(pawn->get_Position().get_File(), pawn->get_Position().get_Rank() + (moveVector << 1));

			if (IsPositionValid(twoStepsForward) && pieceMap.find(twoStepsForward) == pieceMap.end())
			{
				moves.push_back(twoStepsForward);
			}
		}
	}

	return moves;
}

std::vector<Chess::Coordinate> Chess::PawnChecker::GetDiagonalMoves(const std::shared_ptr<Pawn>& pawn, const std::vector<std::shared_ptr<IPiece>>& piecesOnBoard)
{
	ValidatePawn(pawn);

	int moveVector = (pawn->get_ColorAndType().get_Color() == ePieceColor::WHITE) ? 1 : -1;

	std::vector<Coordinate> moves;
	moves.reserve(COUNT_OF_PAWN_WAYS);

	auto pieceMap = CreatePieceMap(piecesOnBoard);

	Coordinate rightDiagonal(pawn->get_Position().get_File() + 1, pawn->get_Position().get_Rank() + moveVector);
	Coordinate leftDiagonal(pawn->get_Position().get_File() - 1, pawn->get_Position().get_Rank() + moveVector);
	Coordinate left(pawn->get_Position().get_File() - 1, pawn->get_Position().get_Rank());
	Coordinate right(pawn->get_Position().get_File() + 1, pawn->get_Position().get_Rank());

	if (IsPositionValid(rightDiagonal))
	{
		if (pieceMap.find(rightDiagonal) != pieceMap.end()
			&& pieceMap.at(rightDiagonal)->get_ColorAndType().get_Color() != pawn->get_ColorAndType().get_Color())
		{
			moves.push_back(rightDiagonal);
		}
		else if (pieceMap.find(right) != pieceMap.end()
			&& std::dynamic_pointer_cast<Pawn>(pieceMap.at(right))->get_CanEnPassant()
			&& pieceMap.at(right)->get_ColorAndType().get_Color() != pawn->get_ColorAndType().get_Color())
		{
			moves.push_back(rightDiagonal);
		}
	}

	if (IsPositionValid(leftDiagonal))
	{
		if (pieceMap.find(leftDiagonal) != pieceMap.end()
			&& pieceMap.at(leftDiagonal)->get_ColorAndType().get_Color() != pawn->get_ColorAndType().get_Color())
		{
			moves.push_back(leftDiagonal);
		}
		else if (pieceMap.find(left) != pieceMap.end()
			&& std::dynamic_pointer_cast<Pawn>(pieceMap.at(left))->get_CanEnPassant()
			&& pieceMap.at(left)->get_ColorAndType().get_Color() != pawn->get_ColorAndType().get_Color())
		{
			moves.push_back(leftDiagonal);
		}
	}

	return moves;
}

void Chess::PawnChecker::ValidatePawn(const std::shared_ptr<Pawn>& pawn)
{
	if (!IsPositionValid(pawn->get_Position()))
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

	auto forwardMoves = FindAllMoves(std::static_pointer_cast<Pawn>(piece), piecesOnBoard);
	auto diagonalMoves = GetDiagonalMoves(std::static_pointer_cast<Pawn>(piece), piecesOnBoard);

	allMoves.insert(allMoves.end(), forwardMoves.begin(), forwardMoves.end());
	allMoves.insert(allMoves.end(), diagonalMoves.begin(), diagonalMoves.end());

	return allMoves;
}
