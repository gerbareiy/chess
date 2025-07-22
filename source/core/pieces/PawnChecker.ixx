module;
#include <expected>
#include <memory>
#include <stdexcept>
#include <vector>
export module Chess.PawnChecker;
import Chess.Coordinate;
import Chess.Counts;
import Chess.eError;
import Chess.ePieceColor;
import Chess.ePieceType;
import Chess.ErrorConverter;
import Chess.IMoveChecker;
import Chess.Pawn;
import Chess.Piece;
import Chess.PieceFinder;
import Chess.PositionChecker;

namespace Chess
{
    export class PawnChecker final : public IMoveChecker
    {
        static std::vector<Coordinate> GetForwardMoves(const std::shared_ptr<Pawn>& pawn, const std::shared_ptr<PieceFinder>& finder)
        {
            auto validatedPawn = ValidatePawn(pawn);
            if (!validatedPawn)
            {
                return {}; // TODO: somehow call checkmate here
            }

            const auto moveVector = (pawn->GetColorAndType().color == ePieceColor::WHITE) ? 1 : -1;

            std::vector<Coordinate> moves;
            moves.reserve(COUNT_OF_PAWN_WAYS);

            Coordinate oneStepForward(pawn->GetPosition().file, pawn->GetPosition().rank + moveVector);

            if (PositionChecker::IsPositionValid(oneStepForward) && !finder->Find(oneStepForward))
            {
                moves.emplace_back(oneStepForward);

                if (pawn->GetIsNotMoved())
                {
                    Coordinate twoStepsForward(pawn->GetPosition().file, pawn->GetPosition().rank + (moveVector << 1));

                    if (PositionChecker::IsPositionValid(twoStepsForward) && !finder->Find(twoStepsForward))
                    {
                        moves.emplace_back(twoStepsForward);
                    }
                }
            }

            return moves;
        }

        static std::vector<Coordinate> GetDiagonalMoves(const std::shared_ptr<Pawn>& pawn, const std::shared_ptr<PieceFinder>& finder)
        {
            auto validatedPawn = ValidatePawn(pawn);
            if (!validatedPawn)
            {
                return {}; // TODO: somehow call checkmate here
            }

            const int moveVector = (pawn->GetColorAndType().color == ePieceColor::WHITE) ? 1 : -1;

            std::vector<Coordinate> moves;
            moves.reserve(COUNT_OF_PAWN_WAYS);

            Coordinate       rightDiagonal(pawn->GetPosition().file + 1, pawn->GetPosition().rank + moveVector);
            Coordinate       leftDiagonal(pawn->GetPosition().file - 1, pawn->GetPosition().rank + moveVector);
            const Coordinate left(pawn->GetPosition().file - 1, pawn->GetPosition().rank);
            const Coordinate right(pawn->GetPosition().file + 1, pawn->GetPosition().rank);

            if (PositionChecker::IsPositionValid(rightDiagonal))
            {
                const auto rightDiagonalPiece = finder->Find(rightDiagonal);
                const auto rightPiece         = finder->Find(right);

                if (rightDiagonalPiece && rightDiagonalPiece->GetColorAndType().color != pawn->GetColorAndType().color)
                {
                    moves.emplace_back(rightDiagonal);
                }
                else if (rightPiece && typeid(*rightPiece) == typeid(Pawn) && std::static_pointer_cast<Pawn>(rightPiece)->GetCanEnPassant()
                         && rightPiece->GetColorAndType().color != pawn->GetColorAndType().color)
                {
                    moves.emplace_back(rightDiagonal);
                }
            }

            if (PositionChecker::IsPositionValid(leftDiagonal))
            {
                const auto leftDiagonalPiece = finder->Find(leftDiagonal);
                const auto leftPiece         = finder->Find(left);

                if (leftDiagonalPiece && leftDiagonalPiece->GetColorAndType().color != pawn->GetColorAndType().color)
                {
                    moves.emplace_back(leftDiagonal);
                }
                else if (leftPiece && typeid(*leftPiece) == typeid(Pawn) && std::static_pointer_cast<Pawn>(leftPiece)->GetCanEnPassant()
                         && leftPiece->GetColorAndType().color != pawn->GetColorAndType().color)
                {
                    moves.emplace_back(leftDiagonal);
                }
            }

            return moves;
        }

        static std::expected<void, std::string> ValidatePawn(const std::shared_ptr<Pawn>& pawn)
        {
            if (!PositionChecker::IsPositionValid(pawn->GetPosition()))
            {
                return std::unexpected(ErrorConverter::ToString(eError::OUT_OF_CHESSBOARD));
            }
            if (pawn->GetColorAndType().color == ePieceColor::NONE)
            {
                return std::unexpected(ErrorConverter::ToString(eError::NOT_CORRECT_PIECE));
            }
        }

    public:
        virtual std::vector<Coordinate> GetMoves(const std::shared_ptr<Piece>& piece, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const override
        {
            if (!piece || typeid(*piece) != typeid(Pawn) || piece->GetColorAndType().type != ePieceType::PAWN)
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
    };
} // namespace Chess
