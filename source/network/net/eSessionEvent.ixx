export module Chess.Net.eSessionEvent;

namespace Chess::Net
{
    export enum class eSessionEvent
    {
        OPPONENT_MOVED,
        BOARD_SYNCED,
        BOARD_CHECKED,
        GAME_OVER,
        UNKNOWN
    };
} // namespace Chess::Net
