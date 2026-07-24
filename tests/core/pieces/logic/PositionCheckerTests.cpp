#include <gtest/gtest.h>
import Chess.Core.Coordinate;
import Chess.Core.PositionChecker;

namespace ServerTests
{
    TEST(PositionCheckerTests, ReturnsTrueForValidBoardPositions)
    {
        EXPECT_TRUE(Chess::Core::PositionChecker::IsInChessboard({ .file = 'A', .rank = 1 }));
        EXPECT_TRUE(Chess::Core::PositionChecker::IsInChessboard({ .file = 'H', .rank = 8 }));
        EXPECT_TRUE(Chess::Core::PositionChecker::IsInChessboard({ .file = 'D', .rank = 4 }));
    }

    TEST(PositionCheckerTests, ReturnsFalseForPositionsOutsideBoard)
    {
        EXPECT_FALSE(Chess::Core::PositionChecker::IsInChessboard({ .file = '@', .rank = 1 }));
        EXPECT_FALSE(Chess::Core::PositionChecker::IsInChessboard({ .file = 'I', .rank = 1 }));

        EXPECT_FALSE(Chess::Core::PositionChecker::IsInChessboard({ .file = 'A', .rank = 0 }));
        EXPECT_FALSE(Chess::Core::PositionChecker::IsInChessboard({ .file = 'A', .rank = 9 }));

        EXPECT_FALSE(Chess::Core::PositionChecker::IsInChessboard({ .file = '@', .rank = 0 }));
        EXPECT_FALSE(Chess::Core::PositionChecker::IsInChessboard({ .file = 'I', .rank = 9 }));
    }
} // namespace ServerTests
