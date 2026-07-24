export module Chess.Core.eGameState;

namespace Chess::Core
{
    export enum class eGameState
    {
        PLAYING,
        CHECK,
        CHECKMATE,
        DRAW
    };
} // namespace Chess::Core
