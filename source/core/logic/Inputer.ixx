module;
#include <boost/signals2.hpp>
export module Chess.Inputer;
import Chess.eInputType;

namespace Chess
{
    export class Inputer
    {
    protected:
        boost::signals2::signal<void(eInputType)> m_signalOnEnter;

    public:
        virtual ~Inputer() = default;

        virtual boost::signals2::connection ConnectOnEnter(const boost::signals2::signal<void(eInputType)>::slot_type& subscriber)
        {
            return m_signalOnEnter.connect(subscriber);
        }
    };
} // namespace Chess
