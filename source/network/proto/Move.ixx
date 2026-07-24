module;
#include "Move.pb.h"
export module Chess.Network.Move;
import Chess.Core.Move;
import Chess.Network.Coordinate;
import Chess.Network.PieceColorAndType;

namespace Chess::Network
{
    export class Move
    {
    public:
        static chess::proto::Move ToProto(const Core::Move& value)
        {
            chess::proto::Move result;
            *result.mutable_from() = Coordinate::ToProto(value.from);
            *result.mutable_to()   = Coordinate::ToProto(value.to);
            result.set_promotion(PieceColorAndType::ToProto(value.promotion));
            return result;
        }

        static Core::Move FromProto(const chess::proto::Move& message)
        {
            return Core::Move{ .from      = Coordinate::FromProto(message.from()),
                               .to        = Coordinate::FromProto(message.to()),
                               .promotion = PieceColorAndType::FromProto(message.promotion()) };
        }
    };
} // namespace Chess::Network
