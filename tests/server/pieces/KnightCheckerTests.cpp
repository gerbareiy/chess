#include <gtest/gtest.h>

#include <algorithm>
#include <memory>
#include <vector>

import Chess.Coordinate;
import Chess.ePieceColor;
import Chess.Knight;
import Chess.KnightChecker;
import Chess.Pawn;
import Chess.Piece;

namespace ServerTests
{
    class KnightCheckerTestHelper
    {
    public:
        static bool Contains(const std::vector<Chess::Coordinate>& coordinates, const Chess::Coordinate& coordinate)
        {
            return std::ranges::contains(coordinates, coordinate);
        }
    };

    TEST(KnightCheckerTests, SkipsOwnPiecesAndBoardEdges)
    {
        const auto knight   = std::make_shared<Chess::Knight>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'B', .rank = 1 });
        const auto ownPiece = std::make_shared<Chess::Pawn>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'D', .rank = 2 });
        const auto opponent = std::make_shared<Chess::Pawn>(Chess::ePieceColor::BLACK, Chess::Coordinate{ .file = 'A', .rank = 3 });
        const std::vector<std::shared_ptr<Chess::Piece>> pieces{ knight, ownPiece, opponent };

        const Chess::KnightChecker checker(knight);
        const auto                 moves = checker.GetMoves(pieces);

        EXPECT_TRUE(KnightCheckerTestHelper::Contains(moves, { .file = 'A', .rank = 3 }));
        EXPECT_TRUE(KnightCheckerTestHelper::Contains(moves, { .file = 'C', .rank = 3 }));
        EXPECT_FALSE(KnightCheckerTestHelper::Contains(moves, { .file = 'D', .rank = 2 }));
        EXPECT_FALSE(KnightCheckerTestHelper::Contains(moves, { .file = '@', .rank = 3 }));
    }
} // namespace ServerTests
