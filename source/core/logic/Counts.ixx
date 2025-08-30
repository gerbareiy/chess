export module Chess.Counts;
import Chess.Sizes;

namespace Chess
{
    export constexpr int COUNT_OF_KINGS_WITH_ONE_COLOR              = 1;

    export constexpr int COUNT_OF_BISHOP_WAYS = CHESSBOARD_SIZE * 2;
    export constexpr int COUNT_OF_KING_WAYS   = 10;
    export constexpr int COUNT_OF_KNIGHT_WAYS = 8;
    export constexpr int COUNT_OF_PAWN_WAYS   = 4;
    export constexpr int COUNT_OF_QUEEN_WAYS  = CHESSBOARD_SIZE * 4;
    export constexpr int COUNT_OF_ROOK_WAYS   = CHESSBOARD_SIZE * 2;

    export constexpr int MAX_COUNT_ELEMENTS                           = 32;
    export constexpr int MAX_MOVES_COUNT_WITHOUT_PAWN_MOVE_AND_TAKING = 75;
} // namespace Chess
