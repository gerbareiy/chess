module;
#include <memory>
export module Console.Chess.Controller;
import Chess.Chessboard;
import Chess.Coordinate;
import Chess.Promoter;

namespace Console::Chess
{
    export class Controller
    {
        std::shared_ptr<::Chess::Chessboard> m_chessboard;

    public:
        explicit Controller(const std::shared_ptr<::Chess::Chessboard>& chessboard)
            : m_chessboard(chessboard)
        {
        }

        bool TrySelectPiece(const ::Chess::Coordinate& from) const
        {
            return m_chessboard->TrySelectPiece(from);
        }

        bool TryMovePiece(const ::Chess::Coordinate& to, const std::shared_ptr<::Chess::Promoter>& promoter) const
        {
            const auto moved = m_chessboard->TryMovePiece(to, promoter);
            return moved;
        }
    };
} // namespace Console::Chess
