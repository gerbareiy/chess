#pragma once

#include "logic/Inputer.h"

namespace Chess
{
    struct Coordinate;

    class HandlerInputer final : public Inputer
    {
        Coordinate EnterCoordinate() const;
        char       EnterFile() const;
        int        EnterRank() const;

    public:
        Coordinate EnterFrom() const;
        Coordinate EnterTo() const;
    };
} // namespace Chess
