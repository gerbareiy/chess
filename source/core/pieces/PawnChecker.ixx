module;
#include <cassert>
#include <expected>
#include <memory>
#include <optional>
#include <stdexcept>
#include <vector>
export module Chess.PawnChecker;
import Chess.Coordinate;
import Chess.CoordinateToPieceFactory;
import Chess.Counts;
import Chess.ePieceColor;
import Chess.ePieceType;
import Chess.IMoveChecker;
import Chess.Pawn;
import Chess.Piece;
import Chess.PieceFinder;
import Chess.PositionChecker;
import Chess.Utils.Exceptions;

namespace Chess
{
    export class PawnChecker final : public IMoveChecker
    {
        std::shared_ptr<Pawn> m_pawn;

        static std::vector<Coordinate> GetForwardMoves(const std::shared_ptr<Pawn>& pawn, const std::shared_ptr<PieceFinder>& finder)
        {
            if (!ValidatePawn(pawn).has_value())
            {
                return {};
            }

            const auto moveVector = pawn->GetColorAndType().color == ePieceColor::WHITE ? 1 : -1;
            auto       forward    = Coordinate(pawn->GetPosition().file, pawn->GetPosition().rank + moveVector);
            if (!PositionChecker::IsInChessboard(forward) || finder->TryFind(forward))
            {
                return {};
            }

            if (!pawn->GetIsNotMoved())
            {
                return { forward };
            }

            auto twoStepsForward = Coordinate(pawn->GetPosition().file, pawn->GetPosition().rank + (moveVector << 1));
            if (!PositionChecker::IsInChessboard(twoStepsForward) || finder->TryFind(twoStepsForward))
            {
                return { forward };
            }
            return { forward, twoStepsForward };
        }

        static std::optional<Coordinate> CalculateDiagonalMove(
            const std::shared_ptr<Pawn>& pawn, const std::shared_ptr<PieceFinder>& finder, Coordinate&& diagonal, const Coordinate& side)
        {
            if (!PositionChecker::IsInChessboard(diagonal))
            {
                return std::nullopt;
            }

            const auto diagonalPiece = finder->TryFind(diagonal);
            const auto sidePiece     = finder->TryFind(side);

            if ((diagonalPiece && diagonalPiece->GetColorAndType().color != pawn->GetColorAndType().color)
                || (sidePiece && std::dynamic_pointer_cast<Pawn>(sidePiece) && std::dynamic_pointer_cast<Pawn>(sidePiece)->GetCanEnPassant()
                    && sidePiece->GetColorAndType().color != pawn->GetColorAndType().color))
            {
                return diagonal;
            }
            return std::nullopt;
        }

        static std::vector<Coordinate> GetDiagonalMoves(const std::shared_ptr<Pawn>& pawn, const std::shared_ptr<PieceFinder>& finder)
        {
            if (!ValidatePawn(pawn).has_value())
            {
                return {};
            }

            const int moveVector = pawn->GetColorAndType().color == ePieceColor::WHITE ? 1 : -1;

            std::vector<Coordinate> result;
            result.reserve(PAWN_WAYS_COUNT);

            auto       rightDiagonal = Coordinate(pawn->GetPosition().file + 1, pawn->GetPosition().rank + moveVector);
            auto       leftDiagonal  = Coordinate(pawn->GetPosition().file - 1, pawn->GetPosition().rank + moveVector);
            const auto right         = Coordinate(pawn->GetPosition().file + 1, pawn->GetPosition().rank);
            const auto left          = Coordinate(pawn->GetPosition().file - 1, pawn->GetPosition().rank);

            auto const rightMove = CalculateDiagonalMove(pawn, finder, std::move(rightDiagonal), right);
            auto const leftMove  = CalculateDiagonalMove(pawn, finder, std::move(leftDiagonal), left);
            if (rightMove.has_value())
            {
                result.push_back(rightMove.value());
            }
            if (leftMove.has_value())
            {
                result.push_back(leftMove.value());
            }
            return result;
        }

        static std::expected<void, std::string> ValidatePawn(const std::shared_ptr<Pawn>& pawn)
        {
            if (!pawn)
            {
                assert(false);
                std::unreachable();
            }
            if (!PositionChecker::IsInChessboard(pawn->GetPosition()))
            {
                return std::unexpected("ChessPiece is out of the Chessboard");
            }
            if (pawn->GetColorAndType().color == ePieceColor::NONE)
            {
                return std::unexpected("Piece is not correct");
            }
            return {};
        }

    public:
        explicit PawnChecker(const std::shared_ptr<Pawn>& pawn)
            : m_pawn(pawn)
        {
        }

        virtual std::vector<Coordinate> GetMoves(const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const override
        {
            if (m_pawn == nullptr)
            {
                throw Utils::PieceIsNullptrException();
            }

            auto       pieceMap      = CoordinateToPieceFactory::Create(piecesOnBoard);
            const auto finder        = std::make_shared<PieceFinder>(std::move(pieceMap));
            auto       forwardMoves  = GetForwardMoves(m_pawn, finder);
            auto       diagonalMoves = GetDiagonalMoves(m_pawn, finder);

            std::vector<Coordinate> result;
            result.reserve(PAWN_WAYS_COUNT);
            result.insert_range(result.end(), std::move(forwardMoves));
            result.insert_range(result.end(), std::move(diagonalMoves));
            return result;
        }
    };
} // namespace Chess
