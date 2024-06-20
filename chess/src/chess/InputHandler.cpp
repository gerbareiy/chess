#include "InputHandler.h"

#include "logic/Sizes.h"

#include <iostream>
#include <string>

Chess::Coordinate Chess::InputHandler::EnterCoordinate()
{
	std::string input{};

	try
	{
		// событие

		char file{};
		std::getline(std::cin, input);
		file = *input.data();

		if (file >= 'a' && file <= 'a' + CHESSBOARD_SIZE - 1)
		{
			file = std::toupper(file);
		}

		// событие

		int rank{};
		std::getline(std::cin, input);
		rank = std::stoi(input);

		return Coordinate(file, rank);
	}
	catch (...)
	{
		return Coordinate(0, 0);
	}
}

Chess::Coordinate Chess::InputHandler::EnterFrom()
{
	// ToDo:: событие

	return EnterCoordinate();
}

Chess::Coordinate Chess::InputHandler::EnterTo()
{
	// ToDo:: событие

	return EnterCoordinate();
}
