#pragma once

#include "Chessboard.h"
#include "logic/eConsoleColor.h"
#include "pieces/logic/ePieceColor.h"

#include <windows.h>

namespace Chess
{
	class ChessboardDisplayer
	{
	private:
		const std::shared_ptr<Chessboard> m_chessboard;

	public:
		ChessboardDisplayer(const std::shared_ptr<Chessboard> chessboard);

	private:
		eConsoleColor GetBackgroundConsoleColor(Coordinate coordinate);
		void GetOriginalConsoleColor(WORD& originalColors);
		eConsoleColor GetTextConsoleColor(Chess::PieceColorAndType& colorAndType, int originalTextColor);
		void SetConsoleColor(eConsoleColor textColor, eConsoleColor backgroundColor) const;

	public:
		// You can choose this default Display
		void Show();
		// Or create own Display by combining some methiods below
		void ShowChessboard();
		void ShowEmpty() const;
		void ShowInvalidMovePrompt(bool isValidMove) const;
		void ShowTakenPieces(ePieceColor color) const;
	};
}
