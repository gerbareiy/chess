#pragma once

namespace Chess
{
    class ICastable
    {
    public:
        virtual ~ICastable() = default;

        virtual bool GetCanMakeCastling() const = 0;
    };
} // namespace Chess
