export module Chess.Core.ICastable;

namespace Chess::Core
{
    export class ICastable
    {
    public:
        virtual ~ICastable() = default;

        virtual bool GetCanMakeCastling() const = 0;
    };
} // namespace Chess::Core
