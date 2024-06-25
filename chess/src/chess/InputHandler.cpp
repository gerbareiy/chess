#include "InputHandler.h"

#include "logic/Sizes.h"

#include <iostream>
#include <string>

Chess::Coordinate Chess::InputHandler::EnterCoordinate() const
{
	auto file = EnterFile();
	auto rank = EnterRank();

	return Coordinate(file, rank);
}

char Chess::InputHandler::EnterFile() const
{
	m_signalEnter("File: ");

	std::string input;
	char file;
	std::getline(std::cin, input);
	file = *input.data();

	// In this code selection we can get bug if CHESSBOARD_SIZE > 26 whan we need to do?
	if (file >= 'a' && file <= 'a' + CHESSBOARD_SIZE - 1)
	{
		file = std::toupper(file);
	}

	return file;
}

int Chess::InputHandler::EnterRank() const
{
	m_signalEnter("Rank: ");

	std::string input;
	int rank;
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

Chess::Coordinate Chess::InputHandler::EnterFrom() const
{
	m_signalEnter("FROM\n");

	return EnterCoordinate();
}

Chess::Coordinate Chess::InputHandler::EnterTo() const
{
	m_signalEnter("TO\n");

	return EnterCoordinate();
}

boost::signals2::connection Chess::InputHandler::ConnectEnter(const boost::signals2::signal<void(std::string)>::slot_type& subscriber)
{
	return m_signalEnter.connect(subscriber);
}
