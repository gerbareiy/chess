#pragma once

#include "Chessboard.h"
#include "eConsoleColor.h"
#include "pieces/logic/ePieceColor.h"

#include <memory>
#include <windows.h>

namespace Chess
{
	class Console
	{
	private:
		const std::shared_ptr<Chessboard> m_chessboard;

	public:
		Console(const std::shared_ptr<Chessboard> chessboard);

	private:
		Coordinate EnterCoordinate();
		void GetOriginalConsoleColor(WORD& originalColors);
		void SetConsoleColor(eConsoleColor textColor, eConsoleColor backgroundColor) const;

	public:
		// You can choose this default Display
		void Display();
		// Or create own Display by combining methiods below
		void DisplayTakenPieces(ePieceColor color) const;
		void DisplayEmpty() const;
		void DisplayChessboard();
		Coordinate EnterFrom();
		Coordinate EnterTo();
		void ShowInvalidMovePrompt(bool isValidMove) const;
	};
}
