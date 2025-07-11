module;
#include "core/logic/Coordinate.h"
#include <boost/signals2.hpp>

#include <memory>
export module Chess.Controller;
import Chess.Chessboard;

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

        bool TryMovePiece(const Coordinate& to) const
        {
            const auto isMoved = m_chessboard->TryMovePiece(to);

            m_signalMove();

            return isMoved;
        }

        boost::signals2::connection ConnectMove(const boost::signals2::signal<void()>::slot_type& subscriber)
        {
            return m_signalMove.connect(subscriber);
        }
    };
} // namespace Chess
