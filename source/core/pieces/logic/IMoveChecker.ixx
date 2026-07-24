module;
#include <memory>
#include <vector>
export module Chess.Core.IMoveChecker;
import Chess.Core.Coordinate;
import Chess.Core.Piece;

namespace Chess::Core
{
    export class IMoveChecker
    {
    public:
        virtual ~IMoveChecker() = default;

        virtual std::vector<Coordinate> GetMoves(const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const = 0;
    };
} // namespace Chess::Core
