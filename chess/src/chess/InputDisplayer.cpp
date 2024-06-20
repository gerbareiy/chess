#include "InputDisplayer.h"

#include <iostream>

void Chess::InputDisplayer::ShowFile() const
{
	std::cout << "File: ";
}

void Chess::InputDisplayer::ShowFrom() const
{
	std::cout << "FROM\n";
}

void Chess::InputDisplayer::ShowRank() const
{
	std::cout << "Rank: ";
}

void Chess::InputDisplayer::ShowTo() const
{
	std::cout << "TO\n";
}
