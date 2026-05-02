module;
#include <memory>
export module Chess.Controller;
import Chess.Chessboard;
import Chess.Coordinate;
import Chess.Promoter;

namespace Chess
{
    export class Controller
    {
        std::shared_ptr<Chessboard> m_chessboard;

    public:
        explicit Controller(const std::shared_ptr<Chessboard>& chessboard)
            : m_chessboard(chessboard)
        {
        }

        bool TryInitPiece(const Coordinate& from) const
        {
            return m_chessboard->TryInitPiece(from);
        }

        bool TryMovePiece(const Coordinate& to, const std::shared_ptr<Promoter>& promoter) const
        {
            const auto moved = m_chessboard->TryMovePiece(to, promoter);
            return moved;
        }
    };
} // namespace Chess
