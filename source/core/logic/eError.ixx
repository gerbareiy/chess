module;
#include <cstdint>
export module Chess.eError;

namespace Chess
{
    export enum class eError : uint8_t
    {
        OUT_OF_CHESSBOARD = 1,
        OUT_OF_COUNT_OF_BISHOP_KNIGHT_ROOK_WITH_ONE_COLOR,
        NOT_CORRECT_PIECE,
        NOT_CORRECT_MOVE
    };
} // namespace Chess
