export module Chess.Counts;
import Chess.Sizes;

namespace Chess
{
    export constexpr int ONE_COLOR_KINGS_COUNT = 1;

    export constexpr int BISHOP_WAYS_COUNT = CHESSBOARD_SIZE * 2;
    export constexpr int KING_WAYS_COUNT   = 10;
    export constexpr int KNIGHT_WAYS_COUNT = 8;
    export constexpr int PAWN_WAYS_COUNT   = 4;
    export constexpr int QUEEN_WAYS_COUNT  = CHESSBOARD_SIZE * 4;
    export constexpr int ROOK_WAYS_COUNT   = CHESSBOARD_SIZE * 2;

    export constexpr int MAX_ELEMENTS_COUNT                           = 32;
    export constexpr int MAX_MOVES_WITHOUT_PAWN_MOVE_AND_TAKING_COUNT = 75;
} // namespace Chess
