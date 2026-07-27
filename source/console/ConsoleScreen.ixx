module;
#include <print>
#ifdef _WIN32
#include <cstdio>
#include <io.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <unistd.h>
#endif
export module Chess.Console.ConsoleScreen;
import Chess.Console.eConsoleColor;

namespace Chess::Console
{
    export class ConsoleScreen
    {
    public:
        static void Clear()
        {
            if (!IsStyled())
            {
                return;
            }
            std::print("\x1b[2J\x1b[H");
        }

        static void ResetColor()
        {
            SetColor(eConsoleColor::DEFAULT, eConsoleColor::DEFAULT);
        }

        static void SetColor(eConsoleColor textColor, eConsoleColor backgroundColor)
        {
            if (!IsStyled())
            {
                return;
            }
            std::print("\x1b[{};{}m", static_cast<int>(textColor), static_cast<int>(backgroundColor) + BACKGROUND_OFFSET);
        }

    private:
        static constexpr int BACKGROUND_OFFSET = 10;

        // Escape sequences go only to a real terminal, so redirected output (pipes, files, the e2e harness) stays clean.
        static bool IsStyled()
        {
            static const bool isStyled = TryEnableEscapeSequences();
            return isStyled;
        }

        static bool TryEnableEscapeSequences()
        {
#ifdef _WIN32
            if (_isatty(_fileno(stdout)) == 0)
            {
                return false;
            }

            const auto handleConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            DWORD      mode          = 0;
            if (handleConsole == INVALID_HANDLE_VALUE || GetConsoleMode(handleConsole, &mode) == 0)
            {
                return false;
            }
            return SetConsoleMode(handleConsole, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0;
#else
            return isatty(STDOUT_FILENO) != 0;
#endif
        }
    };
} // namespace Chess::Console
