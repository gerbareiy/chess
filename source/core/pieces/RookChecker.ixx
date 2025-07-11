module;
#include <memory>
#include <vector>
export module Chess.RookChecker;
import Chess.BishopQueenRookDirectionChecker;
import Chess.Counts;
import Chess.ePieceType;
import Chess.IMoveChecker;
import Chess.Piece;
import Chess.PieceFinder;
import Chess.Rook;

namespace Chess
{
    export class RookChecker final : public IMoveChecker
    {
        static std::vector<Coordinate> FindPossibleMoves(const std::shared_ptr<Rook>& rook, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard)
        {
            std::vector<Coordinate> moves;
            moves.reserve(COUNT_OF_ROOK_WAYS);

            const auto finder     = std::make_shared<PieceFinder>(piecesOnBoard);

            auto first  = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, rook, { -1, 0 });
            auto second = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, rook, { 1, 0 });
            auto third  = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, rook, { 0, -1 });
            auto fourth = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, rook, { 0, 1 });

            moves.insert(moves.end(), first.begin(), first.end());
            moves.insert(moves.end(), second.begin(), second.end());
            moves.insert(moves.end(), third.begin(), third.end());
            moves.insert(moves.end(), fourth.begin(), fourth.end());

            return moves;
        }

    public:
        virtual std::vector<Coordinate> GetMoves(const std::shared_ptr<Piece>& piece, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const override
        {
            if (!piece || typeid(*piece) != typeid(Rook) || piece->GetColorAndType().type != ePieceType::ROOK)
            {
                return {};
            }
            return FindPossibleMoves(std::static_pointer_cast<Rook>(piece), piecesOnBoard);
        }
    };
} // namespace Chess
