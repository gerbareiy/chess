module;
#include <string>
export module Chess.Core.PieceColorConverter;
import Chess.Core.ePieceColor;

namespace Chess::Core
{
    export class PieceColorConverter
    {
    public:
        static std::string ConvertToString(ePieceColor color)
        {
            switch (color)
            {
            case ePieceColor::NONE:
                return " ";
            case ePieceColor::BLACK:
                return "b";
            case ePieceColor::WHITE:
                return "w";
            default:
                return "Unknown color";
            }
        }
    };
} // namespace Chess::Core
