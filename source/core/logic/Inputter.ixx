export module Chess.Core.Inputter;
import Chess.Core.eInputType;
import Chess.Utils.Event;

namespace Chess::Core
{
    export class Inputter
    {
    public:
        virtual ~Inputter() = default;

        Utils::Event<void(eInputType), Inputter> onEnter;

    protected:
        void RaiseOnEnter(eInputType type) const
        {
            onEnter.Invoke(type);
        }
    };
} // namespace Chess::Core
