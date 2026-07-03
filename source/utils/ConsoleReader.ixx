module;
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
            std::string line;
            std::getline(std::cin, line);
            return line;
        }
    };
} // namespace Chess::Utils
