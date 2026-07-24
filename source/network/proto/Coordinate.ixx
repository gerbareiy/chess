module;
#include "Coordinate.pb.h"
#include <cstdint>
export module Chess.Network.Coordinate;
import Chess.Core.Coordinate;

namespace Chess::Network
{
    export class Coordinate
    {
    public:
        static chess::proto::Coordinate ToProto(const Chess::Core::Coordinate& value)
        {
            chess::proto::Coordinate result;
            result.set_file(static_cast<int32_t>(value.file));
            result.set_rank(value.rank);
            return result;
        }

        static Chess::Core::Coordinate FromProto(const chess::proto::Coordinate& message)
        {
            return Chess::Core::Coordinate{ .file = static_cast<char>(message.file()), .rank = message.rank() };
        }
    };
} // namespace Chess::Network
