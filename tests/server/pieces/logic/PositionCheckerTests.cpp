#include <gtest/gtest.h>

import Chess.PositionChecker;

namespace ServerTests
{
    TEST(PositionCheckerTests, AcceptsOnlyCoordinatesInsideStandardBoard)
    {
        EXPECT_TRUE(Chess::PositionChecker::IsInChessboard({ .file = 'A', .rank = 1 }));
        EXPECT_TRUE(Chess::PositionChecker::IsInChessboard({ .file = 'H', .rank = 8 }));
        EXPECT_FALSE(Chess::PositionChecker::IsInChessboard({ .file = '@', .rank = 1 }));
        EXPECT_FALSE(Chess::PositionChecker::IsInChessboard({ .file = 'I', .rank = 8 }));
        EXPECT_FALSE(Chess::PositionChecker::IsInChessboard({ .file = 'A', .rank = 0 }));
        EXPECT_FALSE(Chess::PositionChecker::IsInChessboard({ .file = 'H', .rank = 9 }));
    }
} // namespace ServerTests
