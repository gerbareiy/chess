module;
#include <memory>
#include <stdexcept>
#include <vector>
export module Chess.QueenChecker;
import Chess.BishopQueenRookDirectionChecker;
import Chess.Coordinate;
import Chess.CoordinateToPieceBuilder;
import Chess.Counts;
import Chess.ePieceColor;
import Chess.ePieceType;
import Chess.IMoveChecker;
import Chess.Piece;
import Chess.PieceFinder;
import Chess.Queen;

namespace Chess
{
    export class QueenChecker final : public IMoveChecker
    {
        static std::vector<Coordinate> FindPossibleMoves(Coordinate position, ePieceColor color, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard)
        {
            std::vector<Coordinate> moves;
            moves.reserve(QUEEN_WAYS_COUNT);

            auto       pieceMap = CoordinateToPieceBuilder::Build(piecesOnBoard);
            const auto finder   = std::make_shared<PieceFinder>(std::move(pieceMap));

            auto first   = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, position, color, { -1, -1 });
            auto second  = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, position, color, { -1, 1 });
            auto third   = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, position, color, { 1, -1 });
            auto fourth  = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, position, color, { 1, 1 });
            auto fifth   = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, position, color, { -1, 0 });
            auto sixth   = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, position, color, { 1, 0 });
            auto seventh = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, position, color, { 0, -1 });
            auto eighth  = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, position, color, { 0, 1 });

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
            if (piece == nullptr)
            {
                throw std::logic_error("piece is nullptr");
            }
            return FindPossibleMoves(piece->GetPosition(), piece->GetColorAndType().color, piecesOnBoard);
        }
    };
} // namespace Chess
