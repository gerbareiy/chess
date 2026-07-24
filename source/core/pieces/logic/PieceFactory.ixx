module;
#include <memory>
#include <stdexcept>
export module Chess.Core.PieceFactory;
import Chess.Core.Bishop;
import Chess.Core.Coordinate;
import Chess.Core.ePieceColor;
import Chess.Core.ePieceType;
import Chess.Core.King;
import Chess.Core.Knight;
import Chess.Core.Pawn;
import Chess.Core.Piece;
import Chess.Core.PieceColorAndType;
import Chess.Core.Queen;
import Chess.Core.Rook;

namespace Chess::Core
{
    export class PieceFactory
    {
    public:
        static std::unique_ptr<Piece> Create(PieceColorAndType colorAndType, Coordinate coordinate, const std::shared_ptr<King>& king)
        {
            switch (colorAndType.type)
            {
            case ePieceType::BISHOP:
                return std::make_unique<Bishop>(colorAndType.color, coordinate);
            case ePieceType::KING:
                return std::make_unique<King>(colorAndType.color, coordinate);
            case ePieceType::KNIGHT:
                return std::make_unique<Knight>(colorAndType.color, coordinate);
            case ePieceType::PAWN:
                return std::make_unique<Pawn>(colorAndType.color, coordinate);
            case ePieceType::QUEEN:
                return std::make_unique<Queen>(colorAndType.color, coordinate);
            case ePieceType::ROOK:
                return std::make_unique<Rook>(colorAndType.color, coordinate, king);
            case ePieceType::NONE:
                throw std::invalid_argument("Piece type cannot be NONE");
            default:
                throw std::invalid_argument("Unknown piece type");
            }
        }
    };
} // namespace Chess::Core
