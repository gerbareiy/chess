#pragma once

#include "Chessboard.h"
#include "logic/Coordinate.h"
#include "logic/eConsoleColor.h"
#include "pieces/logic/ePieceColor.h"
#include "pieces/logic/PieceColorAndType.h"

#include <memory>
#include <string>
#include <windows.h>

namespace Chess
{
	class ChessboardDisplayer
	{
	private:
		const std::shared_ptr<Chessboard> m_chessboard;

	public:
		ChessboardDisplayer(const std::shared_ptr<Chessboard>& chessboard);

	private:
		eConsoleColor GetBackgroundConsoleColor(Coordinate coordinate) const;
		std::string GetChessboardFiles() const;
		void GetOriginalConsoleColor(WORD& originalColors) const;
		eConsoleColor GetTextConsoleColor(PieceColorAndType& colorAndType, int originalTextColor) const;
		void SetConsoleColor(eConsoleColor textColor, eConsoleColor backgroundColor) const;
		void ShowChessboardFiles(bool isChessboardSizeOneDigit) const;
		void ShowChessboardRank(int y, bool isChessboardSizeOneDigit) const;
		void ShowChessboardWithCoordinates() const;

	public:
		// You can choose this default Display
		void Show();
		// Or create own Display by combining some methiods below
		void ShowChessboardRowWithRank(int y, int originalTextColor) const;
		void ShowEmpty() const;
		void ShowInvalidMovePrompt(bool isValidMove) const;
		void ShowTakenPieces(ePieceColor color) const;
	};
}
