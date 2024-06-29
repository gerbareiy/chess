#pragma once

#include "Sizes.h"

namespace Chess
{
	constexpr int COUNT_OF_BISHOP_KNIGHT_ROOK_WITH_1_COLOR = 2;

	constexpr int COUNT_OF_BISHOP_WAYS = CHESSBOARD_SIZE * 2;
	constexpr int COUNT_OF_KING_WAYS = 10;
	constexpr int COUNT_OF_KNIGHT_WAYS = 8;
	constexpr int COUNT_OF_PAWN_WAYS = 4;
	constexpr int COUNT_OF_QUEEN_WAYS = CHESSBOARD_SIZE * 4;
	constexpr int COUNT_OF_ROOK_WAYS = CHESSBOARD_SIZE * 2;

	constexpr int MAX_COUNT_ELEMENTS = 32;
	constexpr int MAX_COUNT_SAME_BOARD_POSITION = 5;
	constexpr int MAX_MOVES_COUNT_WITHOUT_PAWN_MOVE_AND_TAKING = 75;
}
