module;
#include <optional>
export module Chess.Network.ServerMessage;
import Chess.Core.eGameState;
import Chess.Core.Move;
import Chess.Network.BoardSnapshot;
import Chess.Network.eSessionEvent;

namespace Chess::Network
{
    export struct ServerMessage
    {
        eSessionEvent                   event;
        std::optional<Core::Move>       move;
        std::optional<BoardSnapshot>    board;
        std::optional<Core::eGameState> finalState;
    };
} // namespace Chess::Network
