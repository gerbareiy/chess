export module Chess.Network.eSessionEvent;

namespace Chess::Network
{
    export enum class eSessionEvent
    {
        OPPONENT_MOVED,
        BOARD_SYNCED,
        BOARD_CHECKED,
        GAME_OVER,
        OPPONENT_LEFT,
        UNKNOWN
    };
} // namespace Chess::Network
