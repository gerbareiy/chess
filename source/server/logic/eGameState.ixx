export module Chess.eGameState;

namespace Chess
{
    export enum class eGameState
    {
        PLAYING,
        CHECK,
        CHECKMATE,
        DRAW
    };
} // namespace Chess
