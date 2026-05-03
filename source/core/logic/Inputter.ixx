module;
#include <boost/signals2.hpp>
export module Chess.Inputer;
import Chess.eInputType;

namespace Chess
{
    export class Inputter
    {
        boost::signals2::signal<void(eInputType)> m_signalOnEnter;

    protected:
        const boost::signals2::signal<void(eInputType)>& GetSignalOnEnter() const
        {
            return m_signalOnEnter;
        }

    public:
        virtual ~Inputter() = default;

        boost::signals2::connection ConnectOnEnter(const std::function<void(eInputType)>& subscriber)
        {
            return m_signalOnEnter.connect(subscriber);
        }
    };
} // namespace Chess
