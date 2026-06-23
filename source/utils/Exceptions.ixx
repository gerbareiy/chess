module;
#include <stdexcept>
export module Chess.Utils.Exceptions;

namespace Chess::Utils
{
    export class PieceIsNullptrException : public std::logic_error
    {
    public:
        PieceIsNullptrException() : std::logic_error("Piece is nullptr") {}
    };

    export class ImpossibleMoveException : public std::logic_error
    {
    public:
        ImpossibleMoveException()
            : std::logic_error("Impossible move")
        {
        }
    };
}
