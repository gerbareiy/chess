module;
#include <boost/signals2.hpp>
#include <cctype>
#include <iostream>
#include <string>
export module Console.Chess.InputHandler;
import Chess.Coordinate;
import Chess.eInputType;
import Chess.Inputer;
import Chess.Utils.ConsoleReader;
import Chess.Utils.Converter;

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

        std::optional<char> EnterFile() const
        {
            GetSignalOnEnter()(::Chess::eInputType::FILE);
            const auto input = ::Chess::Utils::ConsoleReader::ReadLine();
            return NormalizeFileInput(input);
        }

        std::optional<int32_t> EnterRank() const
        {
            GetSignalOnEnter()(::Chess::eInputType::RANK);

            const auto input = ::Chess::Utils::ConsoleReader::ReadLine();
            return ::Chess::Utils::Converter::ToInt32(input);
        }

        ::Chess::Coordinate EnterCoordinate() const
        {
            while (true)
            {
                const auto file = EnterFile();
                const auto rank = EnterRank();

                if (file.has_value() && rank.has_value())
                {
                    return { .file = file.value(), .rank = rank.value() };
                }
            }
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
