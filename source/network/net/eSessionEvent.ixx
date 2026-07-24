export module Chess.Network.eSessionEvent;

namespace Chess::Network
{
    export enum class eSessionEvent
    {
        OPPONENT_MOVED,
        BOARD_SYNCED,
        BOARD_CHECKED,
        GAME_OVER,
        UNKNOWN
    };
} // namespace Chess::Network
