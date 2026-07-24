module;
#include <cstdint>
export module Chess.Core.ePieceType;

namespace Chess::Core
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
} // namespace Chess::Core
