#include "ChessboardDisplayer.h"

#include "logic/Sizes.h"
#include "pieces/logic/PieceTypeConverter.h"

#include <conio.h>
#include <iostream>

Chess::ChessboardDisplayer::ChessboardDisplayer(const std::shared_ptr<Chessboard> chessboard) : m_chessboard(chessboard) { }

eConsoleColor Chess::ChessboardDisplayer::GetBackgroundConsoleColor(Coordinate coordinate)
{
	eConsoleColor color;
	// more if
	if (m_chessboard->get_MoveValidator()->IsCoordinateInPossibleMoves(coordinate))
	{
		color = eConsoleColor::RED;
	}
	else if ((static_cast<int>(coordinate.get_File() + 1) + coordinate.get_Rank()) % 2)
	{
		color = eConsoleColor::BROWN;
	}
	else
	{
		color = eConsoleColor::YELLOW;
	}

	return color;
}

void Chess::ChessboardDisplayer::GetOriginalConsoleColor(WORD& originalColors)
{
	auto handleConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	GetConsoleScreenBufferInfo(handleConsole, &consoleInfo);
	originalColors = consoleInfo.wAttributes;
}

eConsoleColor Chess::ChessboardDisplayer::GetTextConsoleColor(Chess::PieceColorAndType& colorAndType, int originalTextColor)
{
	return colorAndType.get_Color() == ePieceColor::BLACK ? eConsoleColor::BLACK
		: (colorAndType.get_Color() == ePieceColor::WHITE ? eConsoleColor::WHITE : static_cast<eConsoleColor>(originalTextColor));
}

void Chess::ChessboardDisplayer::SetConsoleColor(eConsoleColor textColor, eConsoleColor backgroundColor) const
{
	auto hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (static_cast<int>(backgroundColor) << 4) | static_cast<int>(textColor));
}

void Chess::ChessboardDisplayer::Show()
{
	ShowTakenPieces(ePieceColor::WHITE);
	ShowEmpty();
	ShowChessboard();
	ShowEmpty();
	ShowTakenPieces(ePieceColor::BLACK);
	ShowEmpty();
}

void Chess::ChessboardDisplayer::ShowChessboard()
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
			auto colorAndType = m_chessboard->get_PieceDirector()->GetPieceColorAndType(Coordinate(x, y));
			auto textColor = GetTextConsoleColor(colorAndType, originalTextColor);
			auto background = GetBackgroundConsoleColor(Coordinate(x, y));

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

void Chess::ChessboardDisplayer::ShowEmpty() const
{
	std::cout << '\n';
}

void Chess::ChessboardDisplayer::ShowInvalidMovePrompt(bool isValidMove) const
{
	if (!isValidMove)
	{
		std::cout << "Move is invalid\n";
		std::cout << "Press any key to continue...";
		auto _ = _getch();
	}
}

void Chess::ChessboardDisplayer::ShowTakenPieces(ePieceColor color) const
{
	const auto eatenPieces = m_chessboard->get_PieceDirector()->get_EatenPieces();

	for (const auto& piece : eatenPieces)
	{
		if (piece->get_ColorAndType().get_Color() == color)
		{
			std::cout << PieceTypeConverter::ConvertToString(piece->get_ColorAndType().get_Type());
		}
	}

	std::cout << '\n';
}
