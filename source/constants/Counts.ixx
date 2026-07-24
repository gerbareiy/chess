export module Chess.Constants.Counts;
import Chess.Constants.Sizes;

namespace Chess::Constants::Counts
{
    export constexpr int ONE_COLOR_KINGS_COUNT = 1;

    export constexpr int BISHOP_WAYS_COUNT = Sizes::CHESSBOARD_SIZE * 2;
    export constexpr int KING_WAYS_COUNT   = 10;
    export constexpr int KNIGHT_WAYS_COUNT = 8;
    export constexpr int PAWN_WAYS_COUNT   = 4;
    export constexpr int QUEEN_WAYS_COUNT  = Sizes::CHESSBOARD_SIZE * 4;
    export constexpr int ROOK_WAYS_COUNT   = Sizes::CHESSBOARD_SIZE * 2;

    export constexpr int MAX_ELEMENTS_COUNT = 32;

    export constexpr int MAX_HALF_MOVES_WITHOUT_PAWN_MOVE_AND_TAKING_COUNT = 150;
    export constexpr int MAX_POSSIBLE_PAWN_MOVE_COUNT                      = 2;
} // namespace Chess::Constants::Counts
