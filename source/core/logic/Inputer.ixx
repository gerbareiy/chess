module;
#include <boost/signals2.hpp>

#include <string>
export module Chess.Inputer;

namespace Chess
{
    export class Inputer
    {
    protected:
        boost::signals2::signal<void(std::string)> m_signalEnter;

    public:
        virtual ~Inputer() = default;

        virtual boost::signals2::connection ConnectEnter(const boost::signals2::signal<void(std::string)>::slot_type& subscriber)
        {
            return m_signalEnter.connect(subscriber);
        }
    };
} // namespace Chess
