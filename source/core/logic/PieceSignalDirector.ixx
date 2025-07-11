module;
#include <boost/signals2.hpp>
export module Chess.PieceSignalDirector;

namespace Chess
{
    export class PieceSignalDirector
    {

    private:
        boost::signals2::signal<void()>     m_moveSignal;
        boost::signals2::signal<void(bool)> m_moveWithCheckSignal;

    public:
        boost::signals2::connection ConnectMove(const boost::signals2::signal<void()>::slot_type& subscriber)
        {
            return m_moveSignal.connect(subscriber);
        }

        boost::signals2::connection ConnectMoveWithCheck(const boost::signals2::signal<void(bool)>::slot_type& subscriber)
        {
            return m_moveWithCheckSignal.connect(subscriber);
        }

        void Invite() const
        {
            m_moveSignal();
        }

        void Invite(bool isCheck) const
        {
            m_moveWithCheckSignal(isCheck);
        }
    };
} // namespace Chess
