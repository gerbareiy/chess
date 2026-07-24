#include <algorithm>
#include <gtest/gtest.h>
#include <memory>
#include <vector>
import Chess.Core.Coordinate;
import Chess.Core.ePieceColor;
import Chess.Core.Pawn;
import Chess.Core.Piece;
import Chess.Core.Queen;
import Chess.Core.QueenChecker;
import Chess.Utils.Exceptions;

namespace ServerTests
{
    class QueenCheckerTestHelpers
    {
    public:
        static bool Contains(const std::vector<Chess::Core::Coordinate>& coordinates, const Chess::Core::Coordinate& coordinate)
        {
            return std::ranges::contains(coordinates, coordinate);
        }
    };

    TEST(QueenCheckerTests, MovesStraightAndDiagonallyStoppingAtBlockers)
    {
        const auto queen = std::make_shared<Chess::Core::Queen>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'D', .rank = 4 });
        const auto ownPiece = std::make_shared<Chess::Core::Pawn>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'D', .rank = 6 });
        const auto opponent = std::make_shared<Chess::Core::Pawn>(Chess::Core::ePieceColor::BLACK, Chess::Core::Coordinate{ .file = 'F', .rank = 6 });
        const std::vector<std::shared_ptr<Chess::Core::Piece>> pieces = { queen, ownPiece, opponent };

        const auto moves = Chess::Core::QueenChecker(queen).GetMoves(pieces);

        EXPECT_TRUE(QueenCheckerTestHelpers::Contains(moves, { .file = 'D', .rank = 5 }));
        EXPECT_FALSE(QueenCheckerTestHelpers::Contains(moves, { .file = 'D', .rank = 6 }));
        EXPECT_FALSE(QueenCheckerTestHelpers::Contains(moves, { .file = 'D', .rank = 7 }));

        EXPECT_TRUE(QueenCheckerTestHelpers::Contains(moves, { .file = 'E', .rank = 5 }));
        EXPECT_TRUE(QueenCheckerTestHelpers::Contains(moves, { .file = 'F', .rank = 6 }));
        EXPECT_FALSE(QueenCheckerTestHelpers::Contains(moves, { .file = 'G', .rank = 7 }));

        EXPECT_TRUE(QueenCheckerTestHelpers::Contains(moves, { .file = 'A', .rank = 4 }));
    }

    TEST(QueenCheckerTests, ThrowsOnNullptrQueen)
    {
        const auto checker = Chess::Core::QueenChecker(nullptr);
        EXPECT_THROW(checker.GetMoves({}), Chess::Utils::PieceIsNullptrException);
    }
} // namespace ServerTests
