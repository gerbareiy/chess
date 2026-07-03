export module Chess.Move;
import Chess.Coordinate;
import Chess.ePieceType;

namespace Chess
{
    export struct Move
    {
        Coordinate from;
        Coordinate to;
        ePieceType promotion = ePieceType::NONE;

        bool operator==(const Move& other) const = default;
    };
} // namespace Chess
