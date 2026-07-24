module;
#include <memory>
#include <vector>
export module Chess.Core.BishopChecker;
import Chess.Constants.Counts;
import Chess.Core.Bishop;
import Chess.Core.Coordinate;
import Chess.Core.CoordinateToPieceFactory;
import Chess.Core.DirectionMoveChecker;
import Chess.Core.ePieceType;
import Chess.Core.IMoveChecker;
import Chess.Core.Piece;
import Chess.Core.PieceFinder;
import Chess.Utils.Exceptions;

namespace Chess::Core
{
    export class BishopChecker final : public IMoveChecker
    {
    public:
        explicit BishopChecker(const std::shared_ptr<Bishop>& bishop)
            : bishop_(bishop)
        {
        }

        virtual std::vector<Coordinate> GetMoves(const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const override
        {
            if (bishop_ == nullptr)
            {
                throw Utils::PieceIsNullptrException();
            }

            auto       pieceMap = CoordinateToPieceFactory::Create(piecesOnBoard);
            const auto finder   = std::make_shared<PieceFinder>(std::move(pieceMap));

            auto first  = DirectionMoveChecker::FindPossibleMoves(finder, bishop_->GetPosition(), bishop_->GetColorAndType().color, { -1, -1 });
            auto second = DirectionMoveChecker::FindPossibleMoves(finder, bishop_->GetPosition(), bishop_->GetColorAndType().color, { -1, 1 });
            auto third  = DirectionMoveChecker::FindPossibleMoves(finder, bishop_->GetPosition(), bishop_->GetColorAndType().color, { 1, -1 });
            auto fourth = DirectionMoveChecker::FindPossibleMoves(finder, bishop_->GetPosition(), bishop_->GetColorAndType().color, { 1, 1 });

            std::vector<Coordinate> result;
            result.reserve(Constants::Counts::BISHOP_WAYS_COUNT);
            result.insert_range(result.end(), std::move(first));
            result.insert_range(result.end(), std::move(second));
            result.insert_range(result.end(), std::move(third));
            result.insert_range(result.end(), std::move(fourth));
            return result;
        }

    private:
        std::shared_ptr<Bishop> bishop_;
    };
} // namespace Chess::Core
