#include <algorithm>
#include <gtest/gtest.h>
#include <memory>
#include <vector>
import Chess.Core.Coordinate;
import Chess.Core.ePieceColor;
import Chess.Core.Pawn;
import Chess.Core.Piece;
import Chess.Core.Rook;
import Chess.Core.RookChecker;

namespace ServerTests
{
    class RookCheckerTestHelper
    {
    public:
        static bool Contains(const std::vector<Chess::Core::Coordinate>& coordinates, const Chess::Core::Coordinate& coordinate)
        {
            return std::ranges::contains(coordinates, coordinate);
        }
    };

    TEST(RookCheckerTests, StopsAtOwnPieceAndIncludesOpponentPiece)
    {
        const auto rook     = std::make_shared<Chess::Core::Rook>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'D', .rank = 4 });
        const auto ownPiece = std::make_shared<Chess::Core::Pawn>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'D', .rank = 6 });
        const auto opponent = std::make_shared<Chess::Core::Pawn>(Chess::Core::ePieceColor::BLACK, Chess::Core::Coordinate{ .file = 'B', .rank = 4 });
        const std::vector<std::shared_ptr<Chess::Core::Piece>> pieces = { rook, ownPiece, opponent };

        const auto checker = Chess::Core::RookChecker(rook);
        const auto moves   = checker.GetMoves(pieces);

        EXPECT_TRUE(RookCheckerTestHelper::Contains(moves, { .file = 'B', .rank = 4 }));
        EXPECT_FALSE(RookCheckerTestHelper::Contains(moves, { .file = 'A', .rank = 4 }));
        EXPECT_TRUE(RookCheckerTestHelper::Contains(moves, { .file = 'D', .rank = 5 }));
        EXPECT_FALSE(RookCheckerTestHelper::Contains(moves, { .file = 'D', .rank = 6 }));
        EXPECT_FALSE(RookCheckerTestHelper::Contains(moves, { .file = 'D', .rank = 7 }));
    }
} // namespace ServerTests
