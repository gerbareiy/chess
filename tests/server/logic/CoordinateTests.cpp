#include <gtest/gtest.h>
#include <unordered_map>
import Chess.Coordinate;

namespace ServerTests
{
    TEST(CoordinateTests, EqualityComparesFileAndRank)
    {
        constexpr auto first = Chess::Coordinate{ .file = 'C', .rank = 3 };
        constexpr auto second = Chess::Coordinate{ .file = 'C', .rank = 3 };
        constexpr auto third  = Chess::Coordinate{ .file = 'C', .rank = 4 };

        EXPECT_EQ(first, second);
        EXPECT_NE(first, third);
    }

    TEST(CoordinateTests, UsableAsUnorderedMapKey)
    {
        std::unordered_map<Chess::Coordinate, int> map;
        map[{ .file = 'A', .rank = 1 }] = 10;
        map[{ .file = 'A', .rank = 2 }] = 20;
        map[{ .file = 'A', .rank = 1 }] = 30; // overwrites

        EXPECT_EQ(map.size(), 2u);
        EXPECT_EQ(map.at({ .file = 'A', .rank = 1 }), 30);
        EXPECT_EQ(map.at({ .file = 'A', .rank = 2 }), 20);
    }
} // namespace ServerTests
