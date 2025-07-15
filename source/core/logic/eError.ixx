module;
#include <cstdint>
export module Chess.eError;

namespace Chess
{
    export enum class eError : uint8_t
    {
        OUT_OF_CHESSBOARD = 1,
        NOT_CORRECT_PIECE,
        NOT_CORRECT_MOVE
    };
} // namespace Chess
