module;
#include <format>
#include <stdexcept>
export module Chess.Utils.Exceptions;

namespace Chess::Utils
{
    export class PieceIsNullptrException : public std::logic_error
    {
    public:
        PieceIsNullptrException()
            : logic_error("Piece is nullptr.")
        {
        }
    };

    export class ImpossibleMoveException : public std::logic_error
    {
    public:
        ImpossibleMoveException()
            : logic_error("Impossible move.")
        {
        }
    };

    export class NotEqualSizeException : public std::invalid_argument
    {
    public:
        NotEqualSizeException()
            : invalid_argument("Arguments sizes must be equal.")
        {
        }
    };

    export class EngineException : public std::logic_error
    {
    public:
        explicit EngineException(std::string reason)
            : logic_error(std::format("Engine exception: {}", reason))
        {
        }
    };

    export class VulcanException : public std::logic_error
    {
    public:
        explicit VulcanException(std::string reason)
            : logic_error(std::format("Vulcan error: {}", reason))
        {
        }
    };
} // namespace Chess::Utils
