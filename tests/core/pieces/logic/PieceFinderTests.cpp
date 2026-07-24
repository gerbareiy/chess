#include <gtest/gtest.h>
#include <memory>
#include <vector>
import Chess.Core.Coordinate;
import Chess.Core.CoordinateToPieceFactory;
import Chess.Core.ePieceColor;
import Chess.Core.ePieceType;
import Chess.Core.Knight;
import Chess.Core.Pawn;
import Chess.Core.Piece;
import Chess.Core.PieceColorAndType;
import Chess.Core.PieceFinder;

namespace ServerTests
{
    TEST(PieceFinderTests, FindsPieceByCoordinate)
    {
        const auto knight = std::make_shared<Chess::Core::Knight>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'B', .rank = 1 });
        const auto finder = Chess::Core::PieceFinder(Chess::Core::CoordinateToPieceFactory::Create({ knight }));

        EXPECT_EQ(finder.TryFind(Chess::Core::Coordinate{ .file = 'B', .rank = 1 }), knight);
        EXPECT_EQ(finder.TryFind(Chess::Core::Coordinate{ .file = 'C', .rank = 3 }), nullptr);
    }

    TEST(PieceFinderTests, FindsCoordinateByColorAndType)
    {
        const auto pawn   = std::make_shared<Chess::Core::Pawn>(Chess::Core::ePieceColor::BLACK, Chess::Core::Coordinate{ .file = 'E', .rank = 7 });
        const auto knight = std::make_shared<Chess::Core::Knight>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'B', .rank = 1 });
        const auto finder = Chess::Core::PieceFinder(Chess::Core::CoordinateToPieceFactory::Create({ pawn, knight }));

        const auto found = finder.TryFind(Chess::Core::PieceColorAndType{ Chess::Core::ePieceColor::BLACK, Chess::Core::ePieceType::PAWN });
        ASSERT_TRUE(found.has_value());
        EXPECT_EQ(found.value(), (Chess::Core::Coordinate{ .file = 'E', .rank = 7 }));

        EXPECT_FALSE(finder.TryFind(Chess::Core::PieceColorAndType{ Chess::Core::ePieceColor::WHITE, Chess::Core::ePieceType::QUEEN }).has_value());
    }
} // namespace ServerTests
