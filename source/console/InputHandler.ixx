module;
#include <boost/lexical_cast.hpp>
#include <boost/signals2.hpp>

#include <cctype>
#include <iostream>
#include <string>
export module Console.Chess.InputHandler;
import Chess.Coordinate;
import Chess.eInputType;
import Chess.Inputer;

namespace Console::Chess
{
    export class InputHandler final : public ::Chess::Inputter
    {
        static char NormalizeFileInput(const std::string& input)
        {
            if (input.size() != 1)
            {
                return '\0';
            }
            const auto iter = std::ranges::find_if(input, [](unsigned char c) { return std::isalpha(c); });
            return iter != input.end() ? static_cast<char>(std::toupper(*iter)) : '\0';
        }

        char EnterFile() const
        {
            GetSignalOnEnter()(::Chess::eInputType::FILE);
            std::string input;
            std::getline(std::cin, input);
            return NormalizeFileInput(input);
        }

        int EnterRank() const
        {
            GetSignalOnEnter()(::Chess::eInputType::RANK);

            std::string input;
            int         rank = 0;
            std::getline(std::cin, input);
            if (!boost::conversion::try_lexical_convert(input, rank))
            {
                return 0;
            }
            return rank;
        }

        ::Chess::Coordinate EnterCoordinate() const
        {
            const auto file = EnterFile();
            const auto rank = EnterRank();
            return { .file = file, .rank = rank };
        }

    public:
        ::Chess::Coordinate EnterFrom() const
        {
            GetSignalOnEnter()(::Chess::eInputType::FROM);
            return EnterCoordinate();
        }

        ::Chess::Coordinate EnterTo() const
        {
            GetSignalOnEnter()(::Chess::eInputType::TO);
            return EnterCoordinate();
        }
    };
} // namespace Console::Chess
