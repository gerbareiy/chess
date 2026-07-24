#include <gtest/gtest.h>
#include <memory>
#include <vector>
import Chess.Core.Coordinate;
import Chess.Core.CoordinateToPieceFactory;
import Chess.Core.ePieceColor;
import Chess.Core.Pawn;
import Chess.Core.Piece;

namespace ServerTests
{
    TEST(CoordinateToPieceFactoryTests, MapsEachPieceToItsPosition)
    {
        const auto first  = std::make_shared<Chess::Core::Pawn>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'A', .rank = 2 });
        const auto second = std::make_shared<Chess::Core::Pawn>(Chess::Core::ePieceColor::BLACK, Chess::Core::Coordinate{ .file = 'H', .rank = 7 });

        const auto map = Chess::Core::CoordinateToPieceFactory::Create({ first, second });

        ASSERT_EQ(map.size(), 2u);
        EXPECT_EQ(map.at(Chess::Core::Coordinate{ .file = 'A', .rank = 2 }), first);
        EXPECT_EQ(map.at(Chess::Core::Coordinate{ .file = 'H', .rank = 7 }), second);
    }

    TEST(CoordinateToPieceFactoryTests, EmptyInputYieldsEmptyMap)
    {
        EXPECT_TRUE(Chess::Core::CoordinateToPieceFactory::Create({}).empty());
    }
} // namespace ServerTests
