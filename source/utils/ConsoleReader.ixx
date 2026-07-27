module;
#include <cstdio>
#include <iostream>
#include <string>
export module Chess.Utils.ConsoleReader;

namespace Chess::Utils
{
    export class ConsoleReader
    {
    public:
        static std::string ReadLine()
        {
            // Redirected output is fully buffered, so the prompt that was just printed has to be pushed out before
            // blocking on the answer - otherwise whoever drives this process waits for a prompt that never arrives.
            std::fflush(stdout);

            std::string line;
            std::getline(std::cin, line);
            return line;
        }
    };
} // namespace Chess::Utils
