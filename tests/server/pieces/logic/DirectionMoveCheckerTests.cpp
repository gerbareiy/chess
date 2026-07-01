#include <algorithm>
#include <gtest/gtest.h>
#include <memory>
#include <vector>
import Chess.Coordinate;
import Chess.CoordinateToPieceFactory;
import Chess.DirectionMoveChecker;
import Chess.ePieceColor;
import Chess.Pawn;
import Chess.Piece;
import Chess.PieceFinder;

namespace ServerTests
{
    namespace
    {
        std::shared_ptr<Chess::PieceFinder> MakeFinder(const std::vector<std::shared_ptr<Chess::Piece>>& pieces)
        {
            return std::make_shared<Chess::PieceFinder>(Chess::CoordinateToPieceFactory::Create(pieces));
        }

        bool Contains(const std::vector<Chess::Coordinate>& coordinates, const Chess::Coordinate& coordinate)
        {
            return std::ranges::contains(coordinates, coordinate);
        }
    } // namespace

    TEST(DirectionMoveCheckerTests, SlidesUntilBoardEdge)
    {
        const auto finder = MakeFinder({});
        const auto moves =
            Chess::DirectionMoveChecker::FindPossibleMoves(finder, { .file = 'A', .rank = 1 }, Chess::ePieceColor::WHITE, { 1, 1 });

        EXPECT_EQ(moves.size(), 7u);
        EXPECT_TRUE(Contains(moves, { .file = 'H', .rank = 8 }));
        EXPECT_TRUE(Contains(moves, { .file = 'B', .rank = 2 }));
    }

    TEST(DirectionMoveCheckerTests, StopsBeforeOwnPiece)
    {
        const auto blocker = std::make_shared<Chess::Pawn>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'F', .rank = 4 });
        const auto finder  = MakeFinder({ blocker });

        const auto moves =
            Chess::DirectionMoveChecker::FindPossibleMoves(finder, { .file = 'D', .rank = 4 }, Chess::ePieceColor::WHITE, { 1, 0 });

        EXPECT_TRUE(Contains(moves, { .file = 'E', .rank = 4 }));
        EXPECT_FALSE(Contains(moves, { .file = 'F', .rank = 4 }));
        EXPECT_FALSE(Contains(moves, { .file = 'G', .rank = 4 }));
    }

    TEST(DirectionMoveCheckerTests, IncludesOpponentPieceThenStops)
    {
        const auto opponent = std::make_shared<Chess::Pawn>(Chess::ePieceColor::BLACK, Chess::Coordinate{ .file = 'D', .rank = 6 });
        const auto finder   = MakeFinder({ opponent });

        const auto moves =
            Chess::DirectionMoveChecker::FindPossibleMoves(finder, { .file = 'D', .rank = 4 }, Chess::ePieceColor::WHITE, { 0, 1 });

        EXPECT_TRUE(Contains(moves, { .file = 'D', .rank = 5 }));
        EXPECT_TRUE(Contains(moves, { .file = 'D', .rank = 6 }));
        EXPECT_FALSE(Contains(moves, { .file = 'D', .rank = 7 }));
    }
} // namespace ServerTests
