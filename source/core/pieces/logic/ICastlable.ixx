export module Chess.ICastable;

namespace Chess
{
    export class ICastable
    {
    public:
        virtual ~ICastable() = default;

        virtual bool GetCanMakeCastling() const = 0;
    };
} // namespace Chess
