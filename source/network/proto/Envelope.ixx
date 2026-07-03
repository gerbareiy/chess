module;
#include "Envelope.pb.h"
export module Chess.Proto.Envelope;
import Chess.Move;
import Chess.Proto.Move;

namespace Chess::Proto
{
    export class Envelope
    {
    public:
        static chess::proto::Envelope ToProto(const Chess::Move& move)
        {
            chess::proto::Envelope result;
            *result.mutable_move() = Move::ToProto(move);
            return result;
        }

        static chess::proto::Envelope ToProto(const chess::proto::Chessboard& board)
        {
            chess::proto::Envelope result;
            *result.mutable_board_sync() = board;
            return result;
        }
    };
} // namespace Chess::Proto
