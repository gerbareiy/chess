module;
#include <boost/signals2.hpp>

#include <memory>
export module Chess.Controller;
import Chess.Chessboard;
import Chess.Coordinate;
import Chess.Promoter;

namespace Chess
{
    export class Controller
    {
    private:
        std::shared_ptr<Chessboard> m_chessboard;

        boost::signals2::signal<void()> m_signalMove;

    public:
        explicit Controller(const std::shared_ptr<Chessboard>& chessboard)
            : m_chessboard(chessboard)
        {
        }

        bool TryInitPiece(const Coordinate& from) const
        {
            return m_chessboard->TryInitPiece(from);
        }

        bool TryMovePiece(const Coordinate& to, const std::shared_ptr<Promoter> promoter) const
        {
            const auto moved = m_chessboard->TryMovePiece(to, promoter);

            m_signalMove();

            return moved;
        }

        boost::signals2::connection ConnectMove(const boost::signals2::signal<void()>::slot_type& subscriber)
        {
            return m_signalMove.connect(subscriber);
        }
    };
} // namespace Chess
