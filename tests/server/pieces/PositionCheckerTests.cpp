#include <gtest/gtest.h>
import Chess.Coordinate;
import Chess.PositionChecker;

namespace ServerTests
{
    TEST(PositionCheckerTests, ReturnsTrueForValidBoardPositions)
    {
        EXPECT_TRUE(Chess::PositionChecker::IsInChessboard({ .file = 'A', .rank = 1 }));
        EXPECT_TRUE(Chess::PositionChecker::IsInChessboard({ .file = 'H', .rank = 8 }));
        EXPECT_TRUE(Chess::PositionChecker::IsInChessboard({ .file = 'D', .rank = 4 }));
    }

    TEST(PositionCheckerTests, ReturnsFalseForPositionsOutsideBoard)
    {
        EXPECT_FALSE(Chess::PositionChecker::IsInChessboard({ .file = '@', .rank = 1 }));
        EXPECT_FALSE(Chess::PositionChecker::IsInChessboard({ .file = 'I', .rank = 1 }));

        EXPECT_FALSE(Chess::PositionChecker::IsInChessboard({ .file = 'A', .rank = 0 }));
        EXPECT_FALSE(Chess::PositionChecker::IsInChessboard({ .file = 'A', .rank = 9 }));

        EXPECT_FALSE(Chess::PositionChecker::IsInChessboard({ .file = '@', .rank = 0 }));
        EXPECT_FALSE(Chess::PositionChecker::IsInChessboard({ .file = 'I', .rank = 9 }));
    }
} // namespace ServerTests
