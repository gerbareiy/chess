#pragma once

#include "Chessboard.h"
#include "eConsoleColor.h"

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
		void DisplayEmpty() const;
		void DisplayChessboard();
		Coordinate EnterFrom();
		Coordinate EnterTo();
		void ShowInvalidMovePrompt(bool isValidMove) const;
	};
}
