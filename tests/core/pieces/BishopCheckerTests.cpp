#include <algorithm>
#include <gtest/gtest.h>
#include <memory>
#include <vector>
import Chess.Core.Bishop;
import Chess.Core.BishopChecker;
import Chess.Core.Coordinate;
import Chess.Core.ePieceColor;
import Chess.Core.Pawn;
import Chess.Core.Piece;
import Chess.Utils.Exceptions;

namespace ServerTests
{
    class BishopCheckerTestHelpers
    {
    public:
        static bool Contains(const std::vector<Chess::Core::Coordinate>& coordinates, const Chess::Core::Coordinate& coordinate)
        {
            return std::ranges::contains(coordinates, coordinate);
        }
    };

    TEST(BishopCheckerTests, StopsAtOwnPieceAndIncludesOpponentPiece)
    {
        const auto bishop = std::make_shared<Chess::Core::Bishop>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'D', .rank = 4 });
        const auto ownPiece = std::make_shared<Chess::Core::Pawn>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'F', .rank = 6 });
        const auto opponent = std::make_shared<Chess::Core::Pawn>(Chess::Core::ePieceColor::BLACK, Chess::Core::Coordinate{ .file = 'B', .rank = 2 });
        const std::vector<std::shared_ptr<Chess::Core::Piece>> pieces = { bishop, ownPiece, opponent };

        const auto moves = Chess::Core::BishopChecker(bishop).GetMoves(pieces);

        EXPECT_TRUE(BishopCheckerTestHelpers::Contains(moves, { .file = 'E', .rank = 5 }));
        EXPECT_FALSE(BishopCheckerTestHelpers::Contains(moves, { .file = 'F', .rank = 6 }));
        EXPECT_FALSE(BishopCheckerTestHelpers::Contains(moves, { .file = 'G', .rank = 7 }));

        EXPECT_TRUE(BishopCheckerTestHelpers::Contains(moves, { .file = 'C', .rank = 3 }));
        EXPECT_TRUE(BishopCheckerTestHelpers::Contains(moves, { .file = 'B', .rank = 2 }));
        EXPECT_FALSE(BishopCheckerTestHelpers::Contains(moves, { .file = 'A', .rank = 1 }));

        EXPECT_FALSE(BishopCheckerTestHelpers::Contains(moves, { .file = 'D', .rank = 5 }));
    }

    TEST(BishopCheckerTests, ThrowsOnNullptrBishop)
    {
        const auto checker = Chess::Core::BishopChecker(nullptr);
        EXPECT_THROW(checker.GetMoves({}), Chess::Utils::PieceIsNullptrException);
    }
} // namespace ServerTests
