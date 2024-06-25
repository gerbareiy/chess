#include "ChessboardDisplayer.h"

#include "logic/Sizes.h"
#include "pieces/logic/PieceTypeConverter.h"

#include <conio.h>
#include <iostream>

Chess::ChessboardDisplayer::ChessboardDisplayer(const std::shared_ptr<Chessboard>& chessboard) : m_chessboard(chessboard) { }

eConsoleColor Chess::ChessboardDisplayer::GetBackgroundConsoleColor(Coordinate coordinate) const
{
	eConsoleColor color;
	auto isSquareBlack = (static_cast<int>(coordinate.get_File() + 1) + coordinate.get_Rank()) % 2;

	if (m_chessboard->get_MoveValidator()->IsCoordinateInPieceCanMove(coordinate))
	{
		color = eConsoleColor::CERULEAN;
	}
	else if (m_chessboard->get_MoveValidator()->IsCoordinateInPossibleMoves(coordinate))
	{
		color = eConsoleColor::RED;
	}
	else if (isSquareBlack)
	{
		color = eConsoleColor::BROWN;
	}
	else
	{
		color = eConsoleColor::YELLOW;
	}

	return color;
}

std::string Chess::ChessboardDisplayer::GetChessboardFiles() const
{
	std::string files = "";

	for (auto i = 0; i < CHESSBOARD_SIZE; ++i)
	{
		files += static_cast<char>('A' + i);
	}

	return files;
}

void Chess::ChessboardDisplayer::GetOriginalConsoleColor(WORD& originalColors) const
{
	auto handleConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	GetConsoleScreenBufferInfo(handleConsole, &consoleInfo);
	originalColors = consoleInfo.wAttributes;
}

eConsoleColor Chess::ChessboardDisplayer::GetTextConsoleColor(Chess::PieceColorAndType& colorAndType, int originalTextColor) const
{
	return colorAndType.get_Color() == ePieceColor::BLACK ? eConsoleColor::BLACK
		: (colorAndType.get_Color() == ePieceColor::WHITE ? eConsoleColor::WHITE : static_cast<eConsoleColor>(originalTextColor));
}

void Chess::ChessboardDisplayer::SetConsoleColor(eConsoleColor textColor, eConsoleColor backgroundColor) const
{
	auto hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (static_cast<int>(backgroundColor) << 4) | static_cast<int>(textColor));
}

void Chess::ChessboardDisplayer::ShowChessboardFiles(bool isChessboardSizeOneDigit) const
{
	ShowEmpty();

	std::cout << (isChessboardSizeOneDigit ? "   " : "\t");
	std::cout << GetChessboardFiles();

	ShowEmpty();
	ShowEmpty();
}

void Chess::ChessboardDisplayer::ShowChessboardRank(int y, bool isChessboardSizeOneDigit) const
{
	auto space = (isChessboardSizeOneDigit ? ' ' : '\t');
	std::cout << space << y << space;
}

void Chess::ChessboardDisplayer::ShowChessboardRowWithRank(int y, int originalTextColor) const
{
	for (auto x = 'A'; x < 'A' + CHESSBOARD_SIZE; ++x)
	{
		auto colorAndType = m_chessboard->get_PieceDirector()->GetPieceColorAndType(Coordinate(x, y));
		auto textColor = GetTextConsoleColor(colorAndType, originalTextColor);
		auto background = GetBackgroundConsoleColor(Coordinate(x, y));

		SetConsoleColor(textColor, background);

		std::cout << PieceTypeConverter::ConvertToString(colorAndType.get_Type())[0];
	}
}

void Chess::ChessboardDisplayer::Show()
{
	ShowTakenPieces(ePieceColor::WHITE);
	ShowChessboardWithCoordinates();
	ShowTakenPieces(ePieceColor::BLACK);
}

void Chess::ChessboardDisplayer::ShowChessboardWithCoordinates() const
{
	WORD originalColors;
	GetOriginalConsoleColor(originalColors);

	auto originalTextColor = originalColors & 0x0F;
	auto originalBackgroundColor = (originalColors & 0xF0) >> 4;
	auto isChessboardSizeOneDigit = CHESSBOARD_SIZE < 10;

	ShowChessboardFiles(isChessboardSizeOneDigit);

	for (auto y = CHESSBOARD_SIZE; y > 0; --y, ShowEmpty())
	{
		ShowChessboardRank(y, isChessboardSizeOneDigit);
		ShowChessboardRowWithRank(y, originalTextColor);
		SetConsoleColor(static_cast<eConsoleColor>(originalTextColor), static_cast<eConsoleColor>(originalBackgroundColor));
		ShowChessboardRank(y, isChessboardSizeOneDigit);
	}

	ShowChessboardFiles(isChessboardSizeOneDigit);
}

void Chess::ChessboardDisplayer::ShowEmpty() const
{
	std::cout << '\n';
}

void Chess::ChessboardDisplayer::ShowInvalidMovePrompt(bool isValidMove) const
{
	if (!isValidMove)
	{
		std::cout << "Move is invalid";
		ShowEmpty();
		std::cout << "Press any key to continue...";
		auto _ = _getch();
	}
}

void Chess::ChessboardDisplayer::ShowTakenPieces(ePieceColor color) const
{
	const auto eatenPieces = m_chessboard->get_PieceDirector()->get_EatenPieces();

	ShowEmpty();

	for (const auto& piece : eatenPieces)
	{
		if (piece->get_ColorAndType().get_Color() == color)
		{
			std::cout << PieceTypeConverter::ConvertToString(piece->get_ColorAndType().get_Type());
		}
	}

	ShowEmpty();
}
