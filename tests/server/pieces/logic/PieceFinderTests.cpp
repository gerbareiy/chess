#include <gtest/gtest.h>
#include <memory>
#include <vector>
import Chess.Coordinate;
import Chess.CoordinateToPieceFactory;
import Chess.ePieceColor;
import Chess.ePieceType;
import Chess.Knight;
import Chess.Pawn;
import Chess.Piece;
import Chess.PieceColorAndType;
import Chess.PieceFinder;

namespace ServerTests
{
    TEST(PieceFinderTests, FindsPieceByCoordinate)
    {
        const auto knight = std::make_shared<Chess::Knight>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'B', .rank = 1 });
        const auto finder = Chess::PieceFinder(Chess::CoordinateToPieceFactory::Create({ knight }));

        EXPECT_EQ(finder.TryFind(Chess::Coordinate{ .file = 'B', .rank = 1 }), knight);
        EXPECT_EQ(finder.TryFind(Chess::Coordinate{ .file = 'C', .rank = 3 }), nullptr);
    }

    TEST(PieceFinderTests, FindsCoordinateByColorAndType)
    {
        const auto pawn   = std::make_shared<Chess::Pawn>(Chess::ePieceColor::BLACK, Chess::Coordinate{ .file = 'E', .rank = 7 });
        const auto knight = std::make_shared<Chess::Knight>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'B', .rank = 1 });
        const auto finder = Chess::PieceFinder(Chess::CoordinateToPieceFactory::Create({ pawn, knight }));

        const auto found = finder.TryFind(Chess::PieceColorAndType{ Chess::ePieceColor::BLACK, Chess::ePieceType::PAWN });
        ASSERT_TRUE(found.has_value());
        EXPECT_EQ(found.value(), (Chess::Coordinate{ .file = 'E', .rank = 7 }));

        EXPECT_FALSE(finder.TryFind(Chess::PieceColorAndType{ Chess::ePieceColor::WHITE, Chess::ePieceType::QUEEN }).has_value());
    }
} // namespace ServerTests
