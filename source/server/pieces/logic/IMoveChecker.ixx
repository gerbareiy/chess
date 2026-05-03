module;
#include <memory>
#include <vector>
export module Chess.IMoveChecker;
import Chess.Coordinate;
import Chess.Piece;

namespace Chess
{
    export class IMoveChecker
    {
    public:
        virtual ~IMoveChecker() = default;

        virtual std::vector<Coordinate> GetMoves(const std::shared_ptr<Piece>& piece, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const = 0;
    };
} // namespace Chess
