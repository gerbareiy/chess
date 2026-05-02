module;
#include <boost/lexical_cast.hpp>
#include <boost/signals2.hpp>

#include <cctype>
#include <iostream>
#include <string>
export module Chess.InputHandler;
import Chess.Coordinate;
import Chess.eInputType;
import Chess.Inputer;

namespace Chess
{
    export class InputHandler final : public Inputter
    {
        static char NormalizeFileInput(const std::string& input)
        {
            for (const unsigned char symbol : input)
            {
                if (!std::isspace(symbol) && std::isalpha(symbol))
                {
                    return static_cast<char>(std::toupper(symbol));
                }
            }

            return '\0';
        }

        char EnterFile() const
        {
            m_signalOnEnter(eInputType::FILE);

            std::string input;
            std::getline(std::cin, input);

            return NormalizeFileInput(input);
        }

        int EnterRank() const
        {
            m_signalOnEnter(eInputType::RANK);

            std::string input;
            int         rank;
            std::getline(std::cin, input);
            if (!boost::conversion::try_lexical_convert(input, rank))
            {
                return -1;
            }
            return rank;
        }

        Coordinate EnterCoordinate() const
        {
            const auto file = EnterFile();
            const auto rank = EnterRank();
            return { .file = file, .rank = rank };
        }

    public:
        Coordinate EnterFrom() const
        {
            m_signalOnEnter(eInputType::FROM);
            return EnterCoordinate();
        }

        Coordinate EnterTo() const
        {
            m_signalOnEnter(eInputType::TO);
            return EnterCoordinate();
        }
    };
} // namespace Chess
