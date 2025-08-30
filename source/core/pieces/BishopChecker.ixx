module;
#include <memory>
#include <vector>
export module Chess.BishopChecker;
import Chess.Bishop;
import Chess.BishopQueenRookDirectionChecker;
import Chess.Coordinate;
import Chess.Counts;
import Chess.ePieceType;
import Chess.IMoveChecker;
import Chess.Piece;
import Chess.PieceFinder;

namespace Chess
{
    export class BishopChecker final : public IMoveChecker
    {
    public:
        virtual std::vector<Coordinate> GetMoves(const std::shared_ptr<Piece>& piece, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const override
        {
            std::vector<Coordinate> result;
            result.reserve(COUNT_OF_BISHOP_WAYS);

            const auto finder = std::make_shared<PieceFinder>(piecesOnBoard);

            auto first  = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, piece, { -1, -1 });
            auto second = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, piece, { -1, 1 });
            auto third  = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, piece, { 1, -1 });
            auto fourth = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, piece, { 1, 1 });

            result.insert(result.end(), first.begin(), first.end());
            result.insert(result.end(), second.begin(), second.end());
            result.insert(result.end(), third.begin(), third.end());
            result.insert(result.end(), fourth.begin(), fourth.end());

            return result;
        }
    };
} // namespace Chess
