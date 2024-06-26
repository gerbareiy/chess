#include "HandlerInputer.h"

#include "logic/Sizes.h"

#include <iostream>
#include <string>

Chess::Coordinate Chess::HandlerInputer::EnterCoordinate() const
{
	auto file = EnterFile();
	auto rank = EnterRank();

	return Coordinate(file, rank);
}

char Chess::HandlerInputer::EnterFile() const
{
	m_signalEnter("File: ");

	std::string input;
	char file;
	std::getline(std::cin, input);
	file = *input.data();

	// In this code selection we can get bug if sign not in abc...xyx what we need to do?
	file = std::toupper(file);
	
	return file;
}

int Chess::HandlerInputer::EnterRank() const
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
