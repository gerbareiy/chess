#include <algorithm>
#include <gtest/gtest.h>
#include <memory>
#include <vector>
import Chess.Coordinate;
import Chess.ePieceColor;
import Chess.Pawn;
import Chess.Piece;
import Chess.Rook;
import Chess.RookChecker;

namespace ServerTests
{
    class RookCheckerTestHelper
    {
    public:
        static bool Contains(const std::vector<Chess::Coordinate>& coordinates, const Chess::Coordinate& coordinate)
        {
            return std::ranges::contains(coordinates, coordinate);
        }
    };

    TEST(RookCheckerTests, StopsAtOwnPieceAndIncludesOpponentPiece)
    {
        const auto rook     = std::make_shared<Chess::Rook>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'D', .rank = 4 });
        const auto ownPiece = std::make_shared<Chess::Pawn>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'D', .rank = 6 });
        const auto opponent = std::make_shared<Chess::Pawn>(Chess::ePieceColor::BLACK, Chess::Coordinate{ .file = 'B', .rank = 4 });
        const std::vector<std::shared_ptr<Chess::Piece>> pieces{ rook, ownPiece, opponent };

        const Chess::RookChecker checker(rook);
        const auto               moves = checker.GetMoves(pieces);

        EXPECT_TRUE(RookCheckerTestHelper::Contains(moves, { .file = 'B', .rank = 4 }));
        EXPECT_FALSE(RookCheckerTestHelper::Contains(moves, { .file = 'A', .rank = 4 }));
        EXPECT_TRUE(RookCheckerTestHelper::Contains(moves, { .file = 'D', .rank = 5 }));
        EXPECT_FALSE(RookCheckerTestHelper::Contains(moves, { .file = 'D', .rank = 6 }));
        EXPECT_FALSE(RookCheckerTestHelper::Contains(moves, { .file = 'D', .rank = 7 }));
    }
} // namespace ServerTests
