#include "HandlerInputer.h"

#include "core/logic/Coordinate.h"

#include <iostream>
#include <string>

Chess::Coordinate Chess::HandlerInputer::EnterCoordinate() const
{
    const auto file = EnterFile();
    const auto rank = EnterRank();

    return { file, rank };
}

char Chess::HandlerInputer::EnterFile() const
{
    m_signalEnter("File: ");

    std::string input;
    std::getline(std::cin, input);

    return std::toupper(*input.data());
}

int Chess::HandlerInputer::EnterRank() const
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

Chess::Coordinate Chess::HandlerInputer::EnterFrom() const
{
    m_signalEnter("FROM\n");

    return EnterCoordinate();
}

Chess::Coordinate Chess::HandlerInputer::EnterTo() const
{
    m_signalEnter("TO\n");

    return EnterCoordinate();
}
