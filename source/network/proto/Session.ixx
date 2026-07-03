module;
#include "Session.pb.h"
export module Chess.Proto.Session;
import Chess.eGameState;

namespace Chess::Proto
{
    export class Session
    {
    public:
        static chess::proto::GameState ToProto(Chess::eGameState state)
        {
            switch (state)
            {
            case Chess::eGameState::CHECK:
                return chess::proto::GAME_STATE_CHECK;
            case Chess::eGameState::CHECKMATE:
                return chess::proto::GAME_STATE_CHECKMATE;
            case Chess::eGameState::DRAW:
                return chess::proto::GAME_STATE_DRAW;
            default:
                return chess::proto::GAME_STATE_PLAYING;
            }
        }

        static Chess::eGameState FromProto(chess::proto::GameState state)
        {
            switch (state)
            {
            case chess::proto::GAME_STATE_CHECK:
                return Chess::eGameState::CHECK;
            case chess::proto::GAME_STATE_CHECKMATE:
                return Chess::eGameState::CHECKMATE;
            case chess::proto::GAME_STATE_DRAW:
                return Chess::eGameState::DRAW;
            default:
                return Chess::eGameState::PLAYING;
            }
        }
    };
} // namespace Chess::Proto
