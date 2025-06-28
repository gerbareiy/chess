#pragma once

#include <cstdint>

namespace Chess
{
    enum class ePieceType : uint8_t
    {
        NONE,
        BISHOP,
        KING,
        KNIGHT,
        PAWN,
        QUEEN,
        ROOK
    };
} // namespace Chess
