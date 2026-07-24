module;
#include <boost/signals2.hpp>
export module Chess.Core.Inputter;
import Chess.Core.eInputType;

namespace Chess::Core
{
    export class Inputter
    {
    public:
        virtual ~Inputter() = default;

        boost::signals2::connection ConnectOnEnter(const std::function<void(eInputType)>& subscriber)
        {
            return signalOnEnter_.connect(subscriber);
        }

    protected:
        const boost::signals2::signal<void(eInputType)>& GetSignalOnEnter() const
        {
            return signalOnEnter_;
        }

    private:
        boost::signals2::signal<void(eInputType)> signalOnEnter_;
    };
} // namespace Chess::Core
