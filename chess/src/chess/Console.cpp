#include "Console.h"

#include "pieces/PieceTypeConverter.h"
#include "Sizes.h"

#include <conio.h>
#include <iostream>
#include <limits>
#include <string>

Chess::Console::Console(const std::shared_ptr<Chessboard> chessboard)
	: m_chessboard(chessboard) { }

Chess::Coordinate Chess::Console::EnterCoordinate()
{
	std::string input{};

	try
	{
		std::cout << "File: ";

		char file{};
		std::getline(std::cin, input);
		file = *input.data();

		if (file >= 'a' && file <= 'h')
		{
			file = std::toupper(file);
		}

		std::cout << "Rank: ";

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

void Chess::Console::GetOriginalConsoleColor(WORD& originalColors)
{
	auto handleConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	GetConsoleScreenBufferInfo(handleConsole, &consoleInfo);
	originalColors = consoleInfo.wAttributes;
}

void Chess::Console::SetConsoleColor(eConsoleColor textColor, eConsoleColor backgroundColor) const
{
	auto hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (static_cast<int>(backgroundColor) << 4) | static_cast<int>(textColor));
}

void Chess::Console::DisplayEmpty() const
{
	std::cout << '\n';
}

void Chess::Console::DisplayChessboard()
{
	WORD originalColors;
	GetOriginalConsoleColor(originalColors);

	auto originalTextColor = originalColors & 0x0F;
	auto originalBackgroundColor = (originalColors & 0xF0) >> 4;

	auto isChessboardSizeDigit = CHESSBOARD_SIZE < 10;

	for (auto y = CHESSBOARD_SIZE; y > 0; --y, std::cout << '\n')
	{
		std::cout << y << (isChessboardSizeDigit ? ' ' : '\t');

		for (auto x = 'A'; x < 'A' + CHESSBOARD_SIZE; ++x)
		{
			auto colorAndType = m_chessboard->GetPieceColorAndType(Coordinate(x, y));
			auto textColor = colorAndType.get_Color() == ePieceColor::BLACK ? eConsoleColor::BLACK
				: (colorAndType.get_Color() == ePieceColor::WHITE ? eConsoleColor::WHITE : static_cast<eConsoleColor>(originalTextColor));

			auto background = m_chessboard->IsCoordinateInPossibleMoves(Coordinate(x, y)) ? eConsoleColor::RED :
				((static_cast<int>(x + 1) + y) % 2) ? eConsoleColor::BROWN : eConsoleColor::YELLOW;

			SetConsoleColor(textColor, background);

			std::cout << PieceTypeConverter::ConvertToString(colorAndType.get_Type())[0];
		}
		SetConsoleColor(static_cast<eConsoleColor>(originalTextColor), static_cast<eConsoleColor>(originalBackgroundColor));
	}

	std::cout << '\n';
	std::cout << (isChessboardSizeDigit ? "  " : "\t");

	for (auto i = 0; i < CHESSBOARD_SIZE; ++i)
	{
		std::cout << static_cast<char>('A' + i);
	}

	std::cout << '\n';
}

Chess::Coordinate Chess::Console::EnterFrom()
{
	std::cout << "FROM\n";

	return EnterCoordinate();
}

Chess::Coordinate Chess::Console::EnterTo()
{
	std::cout << "TO\n";

	return EnterCoordinate();
}

void Chess::Console::ShowInvalidMovePrompt(bool isValidMove) const
{
	if (!isValidMove)
	{
		std::cout << "Move is invalid\n";
		std::cout << "Press any key to continue...";
		auto _ = _getch();
	}
}
