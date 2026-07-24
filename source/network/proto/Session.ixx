module;
#include "Session.pb.h"
export module Chess.Network.Session;
import Chess.Core.eGameState;

namespace Chess::Network
{
    export class Session
    {
    public:
        static chess::proto::GameState ToProto(Chess::Core::eGameState state)
        {
            switch (state)
            {
            case Chess::Core::eGameState::CHECK:
                return chess::proto::GAME_STATE_CHECK;
            case Chess::Core::eGameState::CHECKMATE:
                return chess::proto::GAME_STATE_CHECKMATE;
            case Chess::Core::eGameState::DRAW:
                return chess::proto::GAME_STATE_DRAW;
            default:
                return chess::proto::GAME_STATE_PLAYING;
            }
        }

        static Chess::Core::eGameState FromProto(chess::proto::GameState state)
        {
            switch (state)
            {
            case chess::proto::GAME_STATE_CHECK:
                return Chess::Core::eGameState::CHECK;
            case chess::proto::GAME_STATE_CHECKMATE:
                return Chess::Core::eGameState::CHECKMATE;
            case chess::proto::GAME_STATE_DRAW:
                return Chess::Core::eGameState::DRAW;
            default:
                return Chess::Core::eGameState::PLAYING;
            }
        }
    };
} // namespace Chess::Network
