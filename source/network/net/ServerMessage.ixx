module;
#include <optional>
export module Chess.Net.ServerMessage;
import Chess.eGameState;
import Chess.Move;
import Chess.Net.BoardSnapshot;
import Chess.Net.eSessionEvent;

namespace Chess::Net
{
    export struct ServerMessage
    {
        eSessionEvent                event;
        std::optional<Move>          move;
        std::optional<BoardSnapshot> board;
        std::optional<eGameState>    finalState;
    };
} // namespace Chess::Net
