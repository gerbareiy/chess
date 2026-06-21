module;
#include <memory>
#include <stdexcept>
#include <vector>
export module Chess.RookChecker;
import Chess.Coordinate;
import Chess.CoordinateToPieceFactory;
import Chess.Counts;
import Chess.DirectionMoveChecker;
import Chess.ePieceColor;
import Chess.ePieceType;
import Chess.IMoveChecker;
import Chess.Piece;
import Chess.PieceFinder;
import Chess.Rook;

namespace Chess
{
    export class RookChecker final : public IMoveChecker
    {
        std::shared_ptr<Rook> m_rook;

    public:
        explicit RookChecker(const std::shared_ptr<Rook>& rook)
            : m_rook(rook)
        {
        }

        virtual std::vector<Coordinate> GetMoves(const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const override
        {
            if (m_rook == nullptr)
            {
                throw std::logic_error("piece is nullptr");
            }

            auto       pieceMap = CoordinateToPieceFactory::Create(piecesOnBoard);
            const auto finder   = std::make_shared<PieceFinder>(std::move(pieceMap));

            const auto position = m_rook->GetPosition();
            const auto color    = m_rook->GetColorAndType().color;
            auto       first    = DirectionMoveChecker::FindPossibleMoves(finder, position, color, { -1, 0 });
            auto       second   = DirectionMoveChecker::FindPossibleMoves(finder, position, color, { 1, 0 });
            auto       third    = DirectionMoveChecker::FindPossibleMoves(finder, position, color, { 0, -1 });
            auto       fourth   = DirectionMoveChecker::FindPossibleMoves(finder, position, color, { 0, 1 });

            std::vector<Coordinate> moves;
            moves.reserve(ROOK_WAYS_COUNT);
            moves.insert_range(moves.end(), std::move(first));
            moves.insert_range(moves.end(), std::move(second));
            moves.insert_range(moves.end(), std::move(third));
            moves.insert_range(moves.end(), std::move(fourth));
            return moves;
        }
    };
} // namespace Chess
