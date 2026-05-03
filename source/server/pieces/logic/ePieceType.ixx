module;
#include <cstdint>
export module Chess.ePieceType;

namespace Chess
{
    export enum class ePieceType : uint8_t
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
