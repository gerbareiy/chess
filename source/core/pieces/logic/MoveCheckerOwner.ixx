module;
#include <algorithm>
#include <expected>
#include <memory>
#include <optional>
#include <vector>
export module Chess.MoveCheckerOwner;
import Chess.CheckChecker;
import Chess.Coordinate;
import Chess.CoordinateToPieceFactory;
import Chess.IMoveChecker;
import Chess.Piece;
import Chess.PieceFinder;
import Chess.PiecesOnBoardCreator;

namespace Chess
{
    export class MoveCheckerOwner
    {
    public:
        explicit MoveCheckerOwner(const std::shared_ptr<Piece>& piece, const std::shared_ptr<IMoveChecker>& moveChecker)
            : moveChecker_(moveChecker)
            , piece_(piece)
        {
        }

        bool HasFilteredMoves(const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const
        {
            auto moves = moveChecker_->GetMoves(piecesOnBoard);
            return std::ranges::any_of(
                std::move(moves), [this, &piecesOnBoard](const Coordinate& move) { return IsUncheckedMove(move, piecesOnBoard); });
        }

        std::vector<Coordinate> GetFilteredMoves(const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const
        {
            auto notFilteredMoves = moveChecker_->GetMoves(piecesOnBoard);

            std::vector<Coordinate> result;
            result.reserve(notFilteredMoves.size());
            for (auto&& move : notFilteredMoves)
            {
                if (IsUncheckedMove(move, piecesOnBoard))
                {
                    result.push_back(std::move(move));
                }
            }
            return result;
        }

    private:
        std::shared_ptr<IMoveChecker> moveChecker_;
        std::shared_ptr<Piece>        piece_;

        bool IsUncheckedMove(const Coordinate& move, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const
        {
            auto       piecesOnBoardCopy = PiecesOnBoardCreator::Create(piecesOnBoard);
            auto       pieceMap          = CoordinateToPieceFactory::Create(piecesOnBoardCopy);
            const auto finder            = std::make_shared<PieceFinder>(std::move(pieceMap));
            if (const auto capturedPiece = finder->TryFind(move))
            {
                std::erase(piecesOnBoardCopy, capturedPiece);
            }

            auto const targetPieceCopy = finder->TryFind(piece_->GetPosition());
            targetPieceCopy->Move(move);
            return !CheckChecker::IsCheck(piece_->GetColorAndType().color, piecesOnBoardCopy);
        }
    };
} // namespace Chess
