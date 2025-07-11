#include "PawnChecker.h"

#include "Pawn.h"
#include "Piece.h"
#include "core/logic/Coordinate.h"
#include "core/logic/Counts.h"
#include "core/logic/ErrorConverter.h"
#include "core/logic/eError.h"
#include "logic/PieceColorAndType.h"
#include "logic/PieceFinder.h"
#include "logic/PositionChecker.h"
#include "logic/ePieceColor.h"
#include "logic/ePieceType.h"

#include <stdexcept>

std::vector<Chess::Coordinate> Chess::PawnChecker::GetForwardMoves(const std::shared_ptr<Pawn>& pawn, const std::shared_ptr<PieceFinder>& finder)
{
    ValidatePawn(pawn);

    const auto moveVector = (pawn->GetColorAndType().GetColor() == ePieceColor::WHITE) ? 1 : -1;

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

std::vector<Chess::Coordinate> Chess::PawnChecker::GetDiagonalMoves(const std::shared_ptr<Pawn>& pawn, const std::shared_ptr<PieceFinder>& finder)
{
    ValidatePawn(pawn);

    const int moveVector = (pawn->GetColorAndType().GetColor() == ePieceColor::WHITE) ? 1 : -1;

    std::vector<Coordinate> moves;
    moves.reserve(COUNT_OF_PAWN_WAYS);

    Coordinate       rightDiagonal(pawn->GetPosition().GetFile() + 1, pawn->GetPosition().GetRank() + moveVector);
    Coordinate       leftDiagonal(pawn->GetPosition().GetFile() - 1, pawn->GetPosition().GetRank() + moveVector);
    const Coordinate left(pawn->GetPosition().GetFile() - 1, pawn->GetPosition().GetRank());
    const Coordinate right(pawn->GetPosition().GetFile() + 1, pawn->GetPosition().GetRank());

    if (PositionChecker::IsPositionValid(rightDiagonal))
    {
        const auto rightDiagonalPiece = finder->Find(rightDiagonal);
        const auto rightPiece         = finder->Find(right);

        if (rightDiagonalPiece && rightDiagonalPiece->GetColorAndType().GetColor() != pawn->GetColorAndType().GetColor())
        {
            moves.emplace_back(rightDiagonal);
        }
        else if (rightPiece && typeid(*rightPiece) == typeid(Pawn) && std::static_pointer_cast<Pawn>(rightPiece)->GetCanEnPassant()
                 && rightPiece->GetColorAndType().GetColor() != pawn->GetColorAndType().GetColor())
        {
            moves.emplace_back(rightDiagonal);
        }
    }

    if (PositionChecker::IsPositionValid(leftDiagonal))
    {
        const auto leftDiagonalPiece = finder->Find(leftDiagonal);
        const auto leftPiece         = finder->Find(left);

        if (leftDiagonalPiece && leftDiagonalPiece->GetColorAndType().GetColor() != pawn->GetColorAndType().GetColor())
        {
            moves.emplace_back(leftDiagonal);
        }
        else if (leftPiece && typeid(*leftPiece) == typeid(Pawn) && std::static_pointer_cast<Pawn>(leftPiece)->GetCanEnPassant()
                 && leftPiece->GetColorAndType().GetColor() != pawn->GetColorAndType().GetColor())
        {
            moves.emplace_back(leftDiagonal);
        }
    }

    return moves;
}

void Chess::PawnChecker::ValidatePawn(const std::shared_ptr<Pawn>& pawn)
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

std::vector<Chess::Coordinate> Chess::PawnChecker::GetMoves(const std::shared_ptr<Piece>& piece, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const
{
    if (!piece || typeid(*piece) != typeid(Pawn) || piece->GetColorAndType().GetType() != ePieceType::PAWN)
    {
        return {};
    }

    std::vector<Coordinate> allMoves;

    const auto finder        = std::make_shared<PieceFinder>(piecesOnBoard);
    auto       forwardMoves  = GetForwardMoves(std::static_pointer_cast<Pawn>(piece), finder);
    auto       diagonalMoves = GetDiagonalMoves(std::static_pointer_cast<Pawn>(piece), finder);

    allMoves.insert(allMoves.end(), forwardMoves.begin(), forwardMoves.end());
    allMoves.insert(allMoves.end(), diagonalMoves.begin(), diagonalMoves.end());

    return allMoves;
}
