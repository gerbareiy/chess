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
        static std::vector<Coordinate> FindPossibleMoves(const std::shared_ptr<Bishop>&             bishop,
                                                                        const std::vector<std::shared_ptr<Piece>>& piecesOnBoard)
        {
            std::vector<Coordinate> moves;
            moves.reserve(COUNT_OF_BISHOP_WAYS);

            const auto finder = std::make_shared<PieceFinder>(piecesOnBoard);

            auto first  = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, bishop, { -1, -1 });
            auto second = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, bishop, { -1, 1 });
            auto third  = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, bishop, { 1, -1 });
            auto fourth = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, bishop, { 1, 1 });

            moves.insert(moves.end(), first.begin(), first.end());
            moves.insert(moves.end(), second.begin(), second.end());
            moves.insert(moves.end(), third.begin(), third.end());
            moves.insert(moves.end(), fourth.begin(), fourth.end());

            return moves;
        }

    public:
        virtual std::vector<Coordinate> GetMoves(const std::shared_ptr<Piece>& piece, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const override
        {
            if (std::dynamic_pointer_cast<Bishop>(piece))
            {
                return FindPossibleMoves(std::dynamic_pointer_cast<Bishop>(piece), piecesOnBoard);
            }
            return {};
        }
    };
} // namespace Chess
