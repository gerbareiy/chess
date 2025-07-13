module;
#include <boost/signals2.hpp>

#include <iostream>
export module Chess.InputHandler;
import Chess.Coordinate;
import Chess.Inputer;

namespace Chess
{
    export class InputHandler final : public Inputer
    {
        char EnterFile() const
        {
            m_signalEnter("File: ");

            std::string input;
            std::getline(std::cin, input);

            return std::toupper(*input.data());
        }

        int EnterRank() const
        {
            m_signalEnter("Rank: ");

            std::string input;
            int         rank;
            std::getline(std::cin, input);

            try
            {
                rank = std::stoi(input);
            }
            catch (...)
            {
                return 0;
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
            m_signalEnter("FROM\n");

            return EnterCoordinate();
        }

        Coordinate EnterTo() const
        {
            m_signalEnter("TO\n");

            return EnterCoordinate();
        }
    };
} // namespace Chess
