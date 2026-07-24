module;
#include <memory>
#include <vector>
export module Chess.Core.RookChecker;
import Chess.Constants.Counts;
import Chess.Core.Coordinate;
import Chess.Core.CoordinateToPieceFactory;
import Chess.Core.DirectionMoveChecker;
import Chess.Core.ePieceColor;
import Chess.Core.ePieceType;
import Chess.Core.IMoveChecker;
import Chess.Core.Piece;
import Chess.Core.PieceFinder;
import Chess.Core.Rook;
import Chess.Utils.Exceptions;

namespace Chess::Core
{
    export class RookChecker final : public IMoveChecker
    {
    public:
        explicit RookChecker(const std::shared_ptr<Rook>& rook)
            : rook_(rook)
        {
        }

        virtual std::vector<Coordinate> GetMoves(const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const override
        {
            if (rook_ == nullptr)
            {
                throw Utils::PieceIsNullptrException();
            }

            auto       pieceMap = CoordinateToPieceFactory::Create(piecesOnBoard);
            const auto finder   = std::make_shared<PieceFinder>(std::move(pieceMap));

            const auto position = rook_->GetPosition();
            const auto color    = rook_->GetColorAndType().color;
            auto       first    = DirectionMoveChecker::FindPossibleMoves(finder, position, color, { -1, 0 });
            auto       second   = DirectionMoveChecker::FindPossibleMoves(finder, position, color, { 1, 0 });
            auto       third    = DirectionMoveChecker::FindPossibleMoves(finder, position, color, { 0, -1 });
            auto       fourth   = DirectionMoveChecker::FindPossibleMoves(finder, position, color, { 0, 1 });

            std::vector<Coordinate> moves;
            moves.reserve(Constants::Counts::ROOK_WAYS_COUNT);
            moves.insert_range(moves.end(), std::move(first));
            moves.insert_range(moves.end(), std::move(second));
            moves.insert_range(moves.end(), std::move(third));
            moves.insert_range(moves.end(), std::move(fourth));
            return moves;
        }

    private:
        std::shared_ptr<Rook> rook_;
    };
} // namespace Chess::Core
