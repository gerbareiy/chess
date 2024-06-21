#pragma once

#include "Chessboard.h"
#include "logic/eConsoleColor.h"
#include "pieces/logic/ePieceColor.h"

#include <string>
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
		std::string GetChessboardFiles();
		void GetOriginalConsoleColor(WORD& originalColors);
		eConsoleColor GetTextConsoleColor(Chess::PieceColorAndType& colorAndType, int originalTextColor);
		void SetConsoleColor(eConsoleColor textColor, eConsoleColor backgroundColor) const;
		void ShowChessboardFiles(bool isChessboardSizeOneDigit);
		void ShowChessboardRank(int y, bool isChessboardSizeOneDigit);
		void ShowChessboardWithCoordinates();

	public:
		// You can choose this default Display
		void Show();
		// Or create own Display by combining some methiods below
		void ShowChessboardRowWithRank(int y, int originalTextColor);
		void ShowEmpty() const;
		void ShowInvalidMovePrompt(bool isValidMove) const;
		void ShowTakenPieces(ePieceColor color) const;
	};
}
