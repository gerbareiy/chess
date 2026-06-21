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
        std::shared_ptr<IMoveChecker> m_moveChecker;
        std::shared_ptr<Piece>        m_piece;

        bool IsUncheckedMove(const Coordinate& move, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const
        {
            auto       piecesOnBoardCopy = PiecesOnBoardCreator::Create(piecesOnBoard);
            auto       pieceMap          = CoordinateToPieceFactory::Create(piecesOnBoardCopy);
            const auto finder            = std::make_shared<PieceFinder>(std::move(pieceMap));
            if (const auto capturedPiece = finder->TryFind(move))
            {
                std::erase(piecesOnBoardCopy, capturedPiece);
            }

            auto const targetPieceCopy = finder->TryFind(m_piece->GetPosition());
            targetPieceCopy->Move(move);
            return !CheckChecker::IsCheck(m_piece->GetColorAndType().color, piecesOnBoardCopy);
        }

    public:
        explicit MoveCheckerOwner(const std::shared_ptr<Piece>& piece, const std::shared_ptr<IMoveChecker>& moveChecker)
            : m_moveChecker(moveChecker)
            , m_piece(piece)
        {
        }

        bool HasFilteredMoves(const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const
        {
            auto moves = m_moveChecker->GetMoves(piecesOnBoard);
            return std::ranges::any_of(
                std::move(moves), [this, &piecesOnBoard](const Coordinate& move) { return IsUncheckedMove(move, piecesOnBoard); });
        }

        std::vector<Coordinate> GetFilteredMoves(const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const
        {
            auto notFilteredMoves = m_moveChecker->GetMoves(piecesOnBoard);

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
    };
} // namespace Chess
