module;
#include <memory>
#include <vector>
export module Chess.Core.QueenChecker;
import Chess.Constants.Counts;
import Chess.Core.Coordinate;
import Chess.Core.CoordinateToPieceFactory;
import Chess.Core.DirectionMoveChecker;
import Chess.Core.ePieceColor;
import Chess.Core.ePieceType;
import Chess.Core.IMoveChecker;
import Chess.Core.Piece;
import Chess.Core.PieceFinder;
import Chess.Core.Queen;
import Chess.Utils.Exceptions;

namespace Chess::Core
{
    export class QueenChecker final : public IMoveChecker
    {
    public:
        explicit QueenChecker(const std::shared_ptr<Queen>& queen)
            : queen_(queen)
        {
        }

        virtual std::vector<Coordinate> GetMoves(const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const override
        {
            if (queen_ == nullptr)
            {
                throw Utils::PieceIsNullptrException();
            }
            auto       pieceMap = CoordinateToPieceFactory::Create(piecesOnBoard);
            const auto finder   = std::make_shared<PieceFinder>(std::move(pieceMap));

            const auto position = queen_->GetPosition();
            const auto color    = queen_->GetColorAndType().color;
            auto       first    = DirectionMoveChecker::FindPossibleMoves(finder, position, color, { -1, -1 });
            auto       second   = DirectionMoveChecker::FindPossibleMoves(finder, position, color, { -1, 1 });
            auto       third    = DirectionMoveChecker::FindPossibleMoves(finder, position, color, { 1, -1 });
            auto       fourth   = DirectionMoveChecker::FindPossibleMoves(finder, position, color, { 1, 1 });
            auto       fifth    = DirectionMoveChecker::FindPossibleMoves(finder, position, color, { -1, 0 });
            auto       sixth    = DirectionMoveChecker::FindPossibleMoves(finder, position, color, { 1, 0 });
            auto       seventh  = DirectionMoveChecker::FindPossibleMoves(finder, position, color, { 0, -1 });
            auto       eighth   = DirectionMoveChecker::FindPossibleMoves(finder, position, color, { 0, 1 });

            std::vector<Coordinate> result;
            result.reserve(Constants::Counts::QUEEN_WAYS_COUNT);
            result.insert_range(result.end(), std::move(first));
            result.insert_range(result.end(), std::move(second));
            result.insert_range(result.end(), std::move(third));
            result.insert_range(result.end(), std::move(fourth));
            result.insert_range(result.end(), std::move(fifth));
            result.insert_range(result.end(), std::move(sixth));
            result.insert_range(result.end(), std::move(seventh));
            result.insert_range(result.end(), std::move(eighth));
            return result;
        }

    private:
        std::shared_ptr<Queen> queen_;
    };
} // namespace Chess::Core
