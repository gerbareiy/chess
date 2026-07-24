module;
#include "Move.pb.h"
export module Chess.Proto.Move;
import Chess.Move;
import Chess.Proto.Coordinate;
import Chess.Proto.PieceColorAndType;

namespace Chess::Proto
{
    export class Move
    {
    public:
        static chess::proto::Move ToProto(const Chess::Move& value)
        {
            chess::proto::Move result;
            *result.mutable_from() = Coordinate::ToProto(value.from);
            *result.mutable_to()   = Coordinate::ToProto(value.to);
            result.set_promotion(PieceColorAndType::ToProto(value.promotion));
            return result;
        }

        static Chess::Move FromProto(const chess::proto::Move& message)
        {
            return Chess::Move{ .from      = Coordinate::FromProto(message.from()),
                                .to        = Coordinate::FromProto(message.to()),
                                .promotion = PieceColorAndType::FromProto(message.promotion()) };
        }
    };
} // namespace Chess::Proto
