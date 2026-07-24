export module Chess.Client.eServerEvent;

namespace Chess::Client
{
    export enum class eServerEvent
    {
        OPPONENT_MOVED,
        RESYNCED,
        GAME_OVER
    };
} // namespace Chess::Client
