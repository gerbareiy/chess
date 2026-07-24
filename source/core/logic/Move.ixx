export module Chess.Core.Move;
import Chess.Core.Coordinate;
import Chess.Core.ePieceType;

namespace Chess::Core
{
    export struct Move
    {
        Coordinate from;
        Coordinate to;
        ePieceType promotion = ePieceType::NONE;

        bool operator==(const Move& other) const = default;
    };
} // namespace Chess::Core
