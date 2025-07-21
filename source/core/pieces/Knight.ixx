module;
#include <stdexcept>
export module Chess.Knight;
import Chess.eError;
import Chess.ePieceColor;
import Chess.ePieceType;
import Chess.ErrorConverter;
import Chess.Piece;
import Chess.PieceColorAndType;
import Chess.Sizes;

namespace Chess
{
    export class Knight final : public Piece
    {
    public:
        Knight(ePieceColor color, const Coordinate& coordinate)
            : Piece(PieceColorAndType(color, ePieceType::KNIGHT), coordinate)
        {
        }
    };
} // namespace Chess
