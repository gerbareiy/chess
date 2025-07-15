module;
#include <string>
export module Chess.ErrorConverter;
import Chess.eError;

namespace Chess
{
    export class ErrorConverter
    {
    public:
        static std::string ToString(eError error)
        {
            switch (error)
            {
            case eError::OUT_OF_CHESSBOARD:
                return "ChessPiece is out of the Chessboard";
            case eError::NOT_CORRECT_PIECE:
                return "Piece is not correct";
            case eError::NOT_CORRECT_MOVE:
                return "Move is impossible";
            default:
                return "Unknown error";
            }
        }
    };
} // namespace Chess
