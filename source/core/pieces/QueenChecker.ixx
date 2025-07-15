module;
#include <memory>
#include <vector>
export module Chess.QueenChecker;
import Chess.BishopQueenRookDirectionChecker;
import Chess.Coordinate;
import Chess.Counts;
import Chess.ePieceType;
import Chess.IMoveChecker;
import Chess.Piece;
import Chess.PieceFinder;
import Chess.Queen;

namespace Chess
{
    export class QueenChecker final : public IMoveChecker
    {
        static std::vector<Coordinate> FindPossibleMoves(const std::shared_ptr<Queen>& queen, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard)
        {
            std::vector<Coordinate> moves;
            moves.reserve(COUNT_OF_QUEEN_WAYS);

            const auto finder = std::make_shared<PieceFinder>(piecesOnBoard);

            auto first   = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, queen, { -1, -1 });
            auto second  = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, queen, { -1, 1 });
            auto third   = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, queen, { 1, -1 });
            auto fourth  = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, queen, { 1, 1 });
            auto fifth   = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, queen, { -1, 0 });
            auto sixth   = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, queen, { 1, 0 });
            auto seventh = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, queen, { 0, -1 });
            auto eighth  = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, queen, { 0, 1 });

            moves.insert(moves.end(), first.begin(), first.end());
            moves.insert(moves.end(), second.begin(), second.end());
            moves.insert(moves.end(), third.begin(), third.end());
            moves.insert(moves.end(), fourth.begin(), fourth.end());
            moves.insert(moves.end(), fifth.begin(), fifth.end());
            moves.insert(moves.end(), sixth.begin(), sixth.end());
            moves.insert(moves.end(), seventh.begin(), seventh.end());
            moves.insert(moves.end(), eighth.begin(), eighth.end());

            return moves;
        }

    public:
        virtual std::vector<Coordinate> GetMoves(const std::shared_ptr<Piece>& piece, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const override
        {
            if (!piece || typeid(*piece) != typeid(Queen) || piece->GetColorAndType().type != ePieceType::QUEEN)
            {
                return {};
            }

            return FindPossibleMoves(std::static_pointer_cast<Queen>(piece), piecesOnBoard);
        }
    };
} // namespace Chess
