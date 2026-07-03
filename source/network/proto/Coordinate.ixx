module;
#include <cstdint>
#include "Coordinate.pb.h"
export module Chess.Proto.Coordinate;
import Chess.Coordinate;

namespace Chess::Proto
{
    export class Coordinate
    {
    public:
        static chess::proto::Coordinate ToProto(const Chess::Coordinate& value)
        {
            chess::proto::Coordinate result;
            result.set_file(static_cast<int32_t>(value.file));
            result.set_rank(value.rank);
            return result;
        }

        static Chess::Coordinate FromProto(const chess::proto::Coordinate& message)
        {
            return Chess::Coordinate{ .file = static_cast<char>(message.file()), .rank = message.rank() };
        }
    };
} // namespace Chess::Proto
