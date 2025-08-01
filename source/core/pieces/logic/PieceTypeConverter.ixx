module;
#include <string>
export module Chess.PieceTypeConverter;
import Chess.ePieceType;

namespace Chess
{
    export class PieceTypeConverter
    {
    public:
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
            case Chess::ePieceType::ROOK:
                return "Rook";
                break;
            case Chess::ePieceType::QUEEN:
                return "Queen";
                break;
            case Chess::ePieceType::PAWN:
                return "Pawn";
                break;
            case Chess::ePieceType::KNIGHT:
                return "Knight";
                break;
            case Chess::ePieceType::KING:
                return "King";
                break;
            case Chess::ePieceType::BISHOP:
                return "Bishop";
                break;
            case Chess::ePieceType::NONE:
                return "None";
                break;
            default:
                return "Unknown";
                break;
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
            case Chess::ePieceType::ROOK:
                return "rooks";
                break;
            case Chess::ePieceType::QUEEN:
                return "queens";
                break;
            case Chess::ePieceType::PAWN:
                return "pawns";
                break;
            case Chess::ePieceType::KNIGHT:
                return "knights";
                break;
            case Chess::ePieceType::KING:
                return "kings";
                break;
            case Chess::ePieceType::BISHOP:
                return "bishops";
                break;
            case Chess::ePieceType::NONE:
                return "none";
                break;
            default:
                return "unknown";
                break;
            }
        }
    };
} // namespace Chess
