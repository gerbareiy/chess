module;
#include <string>
#include <array>
export module Chess.PieceTypeConverter;
import Chess.ePieceType;

namespace Chess
{
    export class PieceTypeConverter
    {
    public:
        static constexpr std::array pieceTypes = { ePieceType::KING, ePieceType::PAWN, ePieceType::ROOK, ePieceType::QUEEN, ePieceType::KNIGHT, ePieceType::BISHOP };
        /**
         * @param type Chess piece type (e.g., Chess::ePieceType::ROOK)
         * @return short name of the piece (e.g., "R")
         */
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

        /**
         * @param type Chess piece type (e.g., Chess::ePieceType::ROOK)
         * @return normal name of the piece (e.g., "Rook")
         */
        static std::string ConvertToNormalString(ePieceType pieceType)
        {
            switch (pieceType)
            {
            case ePieceType::ROOK:
                return "Rook";
            case ePieceType::QUEEN:
                return "Queen";
            case ePieceType::PAWN:
                return "Pawn";
            case ePieceType::KNIGHT:
                return "Knight";
            case ePieceType::KING:
                return "King";
            case ePieceType::BISHOP:
                return "Bishop";
            case ePieceType::NONE:
                return "None";
            default:
                return "Unknown";
            }
        }
        
        /**
         * @param type Chess piece type (e.g., Chess::ePieceType::ROOK)
         * @return name of the piece that is used in config file (e.g., "rooks")
         */
        static std::string ConvertToConfigString(ePieceType pieceType)
        {
            switch (pieceType)
            {
            case ePieceType::ROOK:
                return "rooks";
            case ePieceType::QUEEN:
                return "queens";
            case ePieceType::PAWN:
                return "pawns";
            case ePieceType::KNIGHT:
                return "knights";
            case ePieceType::KING:
                return "kings";
            case ePieceType::BISHOP:
                return "bishops";
            case ePieceType::NONE:
                return "none";
            default:
                return "unknown";
            }
        }
    };
} // namespace Chess
