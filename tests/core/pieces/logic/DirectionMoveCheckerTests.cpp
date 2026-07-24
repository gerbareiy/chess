#include <algorithm>
#include <gtest/gtest.h>
#include <memory>
#include <vector>
import Chess.Core.Coordinate;
import Chess.Core.CoordinateToPieceFactory;
import Chess.Core.DirectionMoveChecker;
import Chess.Core.ePieceColor;
import Chess.Core.Pawn;
import Chess.Core.Piece;
import Chess.Core.PieceFinder;

namespace ServerTests
{
    class DirectionMoveCheckerTestHelpers
    {
    public:
        static std::shared_ptr<Chess::Core::PieceFinder> MakeFinder(const std::vector<std::shared_ptr<Chess::Core::Piece>>& pieces)
        {
            return std::make_shared<Chess::Core::PieceFinder>(Chess::Core::CoordinateToPieceFactory::Create(pieces));
        }

        static bool Contains(const std::vector<Chess::Core::Coordinate>& coordinates, const Chess::Core::Coordinate& coordinate)
        {
            return std::ranges::contains(coordinates, coordinate);
        }
    };

    TEST(DirectionMoveCheckerTests, SlidesUntilBoardEdge)
    {
        const auto finder = DirectionMoveCheckerTestHelpers::MakeFinder({});
        const auto moves =
            Chess::Core::DirectionMoveChecker::FindPossibleMoves(finder, { .file = 'A', .rank = 1 }, Chess::Core::ePieceColor::WHITE, { 1, 1 });

        EXPECT_EQ(moves.size(), 7u);
        EXPECT_TRUE(DirectionMoveCheckerTestHelpers::Contains(moves, { .file = 'H', .rank = 8 }));
        EXPECT_TRUE(DirectionMoveCheckerTestHelpers::Contains(moves, { .file = 'B', .rank = 2 }));
    }

    TEST(DirectionMoveCheckerTests, StopsBeforeOwnPiece)
    {
        const auto blocker = std::make_shared<Chess::Core::Pawn>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'F', .rank = 4 });
        const auto finder  = DirectionMoveCheckerTestHelpers::MakeFinder({ blocker });

        const auto moves =
            Chess::Core::DirectionMoveChecker::FindPossibleMoves(finder, { .file = 'D', .rank = 4 }, Chess::Core::ePieceColor::WHITE, { 1, 0 });

        EXPECT_TRUE(DirectionMoveCheckerTestHelpers::Contains(moves, { .file = 'E', .rank = 4 }));
        EXPECT_FALSE(DirectionMoveCheckerTestHelpers::Contains(moves, { .file = 'F', .rank = 4 }));
        EXPECT_FALSE(DirectionMoveCheckerTestHelpers::Contains(moves, { .file = 'G', .rank = 4 }));
    }

    TEST(DirectionMoveCheckerTests, IncludesOpponentPieceThenStops)
    {
        const auto opponent = std::make_shared<Chess::Core::Pawn>(Chess::Core::ePieceColor::BLACK, Chess::Core::Coordinate{ .file = 'D', .rank = 6 });
        const auto finder   = DirectionMoveCheckerTestHelpers::MakeFinder({ opponent });

        const auto moves =
            Chess::Core::DirectionMoveChecker::FindPossibleMoves(finder, { .file = 'D', .rank = 4 }, Chess::Core::ePieceColor::WHITE, { 0, 1 });

        EXPECT_TRUE(DirectionMoveCheckerTestHelpers::Contains(moves, { .file = 'D', .rank = 5 }));
        EXPECT_TRUE(DirectionMoveCheckerTestHelpers::Contains(moves, { .file = 'D', .rank = 6 }));
        EXPECT_FALSE(DirectionMoveCheckerTestHelpers::Contains(moves, { .file = 'D', .rank = 7 }));
    }
} // namespace ServerTests
