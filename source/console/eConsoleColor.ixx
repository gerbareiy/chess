export module Chess.Console.eConsoleColor;

namespace Chess::Console
{
    // ANSI SGR foreground codes; the matching background code is the same value plus 10.
    export enum class eConsoleColor
    {
        BLACK    = 30,
        DARK_RED = 31,
        GREEN    = 32,
        BROWN    = 33,
        BLUE     = 34,
        CERULEAN = 36,
        DEFAULT  = 39,
        GRAY     = 90,
        RED      = 91,
        YELLOW   = 93,
        WHITE    = 97
    };
} // namespace Chess::Console
