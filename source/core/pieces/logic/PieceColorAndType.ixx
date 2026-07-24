export module Chess.Core.PieceColorAndType;
import Chess.Core.ePieceColor;
import Chess.Core.ePieceType;

namespace Chess::Core
{
    export struct PieceColorAndType
    {
        ePieceColor color = ePieceColor::NONE;
        ePieceType  type  = ePieceType::NONE;

        bool operator==(const PieceColorAndType& other) const = default;
    };
} // namespace Chess::Core
