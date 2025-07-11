module;
#include <string>
export module Chess.PieceTypeConverter;
import Chess.ePieceType;

namespace Chess
{
    export class PieceTypeConverter
    {
    public:
        static std::string ConvertToString(ePieceType type)
        {
            switch (type)
            {
            case ePieceType::NONE:
                return " ";
            case ePieceType::BISHOP:
                return "B";
            case ePieceType::KING:
                return "K";
            case ePieceType::KNIGHT:
                return "k";
            case ePieceType::PAWN:
                return "P";
            case ePieceType::QUEEN:
                return "Q";
            case ePieceType::ROOK:
                return "R";
            default:
                return "Unknown piece";
            }
        }
    };
} // namespace Chess
