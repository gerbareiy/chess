#pragma once

namespace Chess
{
    class ICastable
    {
    public:
        virtual ~ICastable() = default;

    public:
        virtual bool GetCanMakeCastling() const = 0;
    };
} // namespace Chess
