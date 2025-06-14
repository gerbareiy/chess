#pragma once

#include "logic/eConsoleColor.h"
#include "pieces/logic/ePieceColor.h"

#include <memory>
#include <string>
#include <windows.h>

namespace Chess
{
	class Chessboard;
	struct Coordinate;
	struct PieceColorAndType;

	class ChessboardDisplayer
	{
	private:
		std::shared_ptr<Chessboard> const m_chessboard;

	public:
		ChessboardDisplayer(std::shared_ptr<Chessboard> const& chessboard);

	private:
		Console::eConsoleColor GetBackgroundConsoleColor(Coordinate const& coordinate) const;
		std::string GetChessboardFiles() const;
		void GetOriginalConsoleColor(WORD& originalColors) const;
		Console::eConsoleColor GetTextConsoleColor(PieceColorAndType& colorAndType, int originalTextColor) const;
		void SetConsoleColor(Console::eConsoleColor textColor, Console::eConsoleColor backgroundColor) const;
		void ShowChessboardFiles(bool isChessboardSizeOneDigit) const;
		void ShowChessboardRank(int y, bool isChessboardSizeOneDigit) const;
		void ShowChessboardWithCoordinates() const;

	public:
		// You can choose this default Display
		void Show();
		// Or create own Display by combining some methiods below
		void ShowChessboardRowWithRank(int y, int originalTextColor) const;
		void ShowEmpty() const;
		void ShowTakenPieces(ePieceColor color) const;
	};
}
