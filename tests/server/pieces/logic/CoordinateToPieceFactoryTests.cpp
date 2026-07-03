#include <gtest/gtest.h>
#include <memory>
#include <vector>
import Chess.Coordinate;
import Chess.CoordinateToPieceFactory;
import Chess.ePieceColor;
import Chess.Pawn;
import Chess.Piece;

namespace ServerTests
{
    TEST(CoordinateToPieceFactoryTests, MapsEachPieceToItsPosition)
    {
        const auto first  = std::make_shared<Chess::Pawn>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'A', .rank = 2 });
        const auto second = std::make_shared<Chess::Pawn>(Chess::ePieceColor::BLACK, Chess::Coordinate{ .file = 'H', .rank = 7 });

        const auto map = Chess::CoordinateToPieceFactory::Create({ first, second });

        ASSERT_EQ(map.size(), 2u);
        EXPECT_EQ(map.at(Chess::Coordinate{ .file = 'A', .rank = 2 }), first);
        EXPECT_EQ(map.at(Chess::Coordinate{ .file = 'H', .rank = 7 }), second);
    }

    TEST(CoordinateToPieceFactoryTests, EmptyInputYieldsEmptyMap)
    {
        EXPECT_TRUE(Chess::CoordinateToPieceFactory::Create({}).empty());
    }
} // namespace ServerTests
