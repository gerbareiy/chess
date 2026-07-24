#include <algorithm>
#include <gtest/gtest.h>
#include <memory>
#include <vector>
import Chess.Core.Coordinate;
import Chess.Core.ePieceColor;
import Chess.Core.Knight;
import Chess.Core.KnightChecker;
import Chess.Core.Pawn;
import Chess.Core.Piece;

namespace ServerTests
{
    class KnightCheckerTestHelper
    {
    public:
        static bool Contains(const std::vector<Chess::Core::Coordinate>& coordinates, const Chess::Core::Coordinate& coordinate)
        {
            return std::ranges::contains(coordinates, coordinate);
        }
    };

    TEST(KnightCheckerTests, SkipsOwnPiecesAndBoardEdges)
    {
        const auto knight = std::make_shared<Chess::Core::Knight>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'B', .rank = 1 });
        const auto ownPiece = std::make_shared<Chess::Core::Pawn>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'D', .rank = 2 });
        const auto opponent = std::make_shared<Chess::Core::Pawn>(Chess::Core::ePieceColor::BLACK, Chess::Core::Coordinate{ .file = 'A', .rank = 3 });
        const std::vector<std::shared_ptr<Chess::Core::Piece>> pieces = { knight, ownPiece, opponent };

        const auto checker = Chess::Core::KnightChecker(knight);
        const auto moves   = checker.GetMoves(pieces);

        EXPECT_TRUE(KnightCheckerTestHelper::Contains(moves, { .file = 'A', .rank = 3 }));
        EXPECT_TRUE(KnightCheckerTestHelper::Contains(moves, { .file = 'C', .rank = 3 }));
        EXPECT_FALSE(KnightCheckerTestHelper::Contains(moves, { .file = 'D', .rank = 2 }));
        EXPECT_FALSE(KnightCheckerTestHelper::Contains(moves, { .file = '@', .rank = 3 }));
    }
} // namespace ServerTests
