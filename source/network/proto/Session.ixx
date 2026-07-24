module;
#include "Session.pb.h"
export module Chess.Network.Session;
import Chess.Core.eGameState;

namespace Chess::Network
{
    export class Session
    {
    public:
        static chess::proto::GameState ToProto(Core::eGameState state)
        {
            switch (state)
            {
            case Core::eGameState::CHECK:
                return chess::proto::GAME_STATE_CHECK;
            case Core::eGameState::CHECKMATE:
                return chess::proto::GAME_STATE_CHECKMATE;
            case Core::eGameState::DRAW:
                return chess::proto::GAME_STATE_DRAW;
            default:
                return chess::proto::GAME_STATE_PLAYING;
            }
        }

        static Core::eGameState FromProto(chess::proto::GameState state)
        {
            switch (state)
            {
            case chess::proto::GAME_STATE_CHECK:
                return Core::eGameState::CHECK;
            case chess::proto::GAME_STATE_CHECKMATE:
                return Core::eGameState::CHECKMATE;
            case chess::proto::GAME_STATE_DRAW:
                return Core::eGameState::DRAW;
            default:
                return Core::eGameState::PLAYING;
            }
        }
    };
} // namespace Chess::Network
