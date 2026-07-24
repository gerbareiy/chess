module;
#include <boost/signals2.hpp>
#include <cctype>
#include <iostream>
#include <string>
export module Chess.Console.InputHandler;
import Chess.Core.Coordinate;
import Chess.Core.eInputType;
import Chess.Core.Inputter;
import Chess.Utils.ConsoleReader;
import Chess.Utils.Converter;

namespace Chess::Console
{
    export class InputHandler final : public ::Chess::Core::Inputter
    {
    public:
        ::Chess::Core::Coordinate EnterFrom() const
        {
            GetSignalOnEnter()(::Chess::Core::eInputType::FROM);
            return EnterCoordinate();
        }

        ::Chess::Core::Coordinate EnterTo() const
        {
            GetSignalOnEnter()(::Chess::Core::eInputType::TO);
            return EnterCoordinate();
        }

    private:
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
            GetSignalOnEnter()(::Chess::Core::eInputType::FILE);
            const auto input = ::Chess::Utils::ConsoleReader::ReadLine();
            return NormalizeFileInput(input);
        }

        std::optional<int32_t> EnterRank() const
        {
            GetSignalOnEnter()(::Chess::Core::eInputType::RANK);

            const auto input = ::Chess::Utils::ConsoleReader::ReadLine();
            return ::Chess::Utils::Converter::ToInt32(input);
        }

        ::Chess::Core::Coordinate EnterCoordinate() const
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
    };
} // namespace Chess::Console
