export module Chess.PieceColorAndType;
import Chess.ePieceColor;
import Chess.ePieceType;

namespace Chess
{
    export struct PieceColorAndType
    {
        ePieceColor color = ePieceColor::NONE;
        ePieceType  type  = ePieceType::NONE;

        bool operator==(const PieceColorAndType& other) const = default;
    };
} // namespace Chess
