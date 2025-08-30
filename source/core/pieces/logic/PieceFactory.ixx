module;
#include <memory>
#include <stdexcept>
export module Chess.PieceFactory;
import Chess.Bishop;
import Chess.Coordinate;
import Chess.ePieceColor;
import Chess.ePieceType;
import Chess.King;
import Chess.Knight;
import Chess.Pawn;
import Chess.Piece;
import Chess.PieceColorAndType;
import Chess.Queen;
import Chess.Rook;

namespace Chess
{
    export class PieceFactory
    {
    public:
        static std::shared_ptr<Piece> Create(PieceColorAndType colorAndType, Coordinate coordinate, const std::shared_ptr<King>& king)
        {
            switch (colorAndType.type)
            {
            case ePieceType::BISHOP:
                return std::make_shared<Bishop>(colorAndType.color, coordinate);
            case ePieceType::KING:
                return std::make_shared<King>(colorAndType.color, coordinate);
            case ePieceType::KNIGHT:
                return std::make_shared<Knight>(colorAndType.color, coordinate);
            case ePieceType::PAWN:
                return std::make_shared<Pawn>(colorAndType.color, coordinate);
            case ePieceType::QUEEN:
                return std::make_shared<Queen>(colorAndType.color, coordinate);
            case ePieceType::ROOK:
                return std::make_shared<Rook>(colorAndType.color, coordinate, king);
            case ePieceType::NONE:
                return nullptr;
            default:
                throw std::invalid_argument("Unknown piece type");
            }
        }
    };
} // namespace Chess
