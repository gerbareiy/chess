#pragma once

#include "Chessboard.h"
#include "logic/eConsoleColor.h"
#include "pieces/logic/ePieceColor.h"

#include <memory>
#include <windows.h>

namespace Chess
{
	class InputHandler
	{
	private:
		Coordinate EnterCoordinate();

	public:
		Coordinate EnterFrom();
		Coordinate EnterTo();
	};
}
