#include <algorithm>
#include <gtest/gtest.h>
#include <memory>
#include <vector>
import Chess.Core.Bishop;
import Chess.Core.Coordinate;
import Chess.Core.ePieceColor;
import Chess.Core.Knight;
import Chess.Core.Pawn;
import Chess.Core.PawnChecker;
import Chess.Core.Piece;
import Chess.Core.PieceTakeLocator;

namespace ServerTests
{
    class PawnCheckerTestHelper
    {
    public:
        static bool Contains(const std::vector<Chess::Core::Coordinate>& coordinates, const Chess::Core::Coordinate& coordinate)
        {
            return std::ranges::contains(coordinates, coordinate);
        }
    };

    TEST(PawnCheckerTests, ReturnsForwardAndCaptureMovesForWhitePawn)
    {
        const auto pawn = std::make_shared<Chess::Core::Pawn>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'D', .rank = 2 });
        const auto opponent =
            std::make_shared<Chess::Core::Knight>(Chess::Core::ePieceColor::BLACK, Chess::Core::Coordinate{ .file = 'E', .rank = 3 });
        const auto ownPiece =
            std::make_shared<Chess::Core::Bishop>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'C', .rank = 3 });
        const std::vector<std::shared_ptr<Chess::Core::Piece>> pieces = { pawn, opponent, ownPiece };

        const auto checker = Chess::Core::PawnChecker(pawn);
        const auto moves   = checker.GetMoves(pieces);

        EXPECT_TRUE(PawnCheckerTestHelper::Contains(moves, { .file = 'D', .rank = 3 }));
        EXPECT_TRUE(PawnCheckerTestHelper::Contains(moves, { .file = 'D', .rank = 4 }));
        EXPECT_TRUE(PawnCheckerTestHelper::Contains(moves, { .file = 'E', .rank = 3 }));
        EXPECT_FALSE(PawnCheckerTestHelper::Contains(moves, { .file = 'C', .rank = 3 }));
    }

    TEST(PawnCheckerTests, SupportsEnPassantCaptureSquare)
    {
        const auto pawn     = std::make_shared<Chess::Core::Pawn>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'E', .rank = 5 });
        const auto opponent = std::make_shared<Chess::Core::Pawn>(Chess::Core::ePieceColor::BLACK, Chess::Core::Coordinate{ .file = 'D', .rank = 7 });
        opponent->Move({ .file = 'D', .rank = 5 });
        const std::vector<std::shared_ptr<Chess::Core::Piece>> pieces = { pawn, opponent };

        const auto checker = Chess::Core::PawnChecker(pawn);
        const auto moves   = checker.GetMoves(pieces);

        EXPECT_TRUE(PawnCheckerTestHelper::Contains(moves, { .file = 'D', .rank = 6 }));
        EXPECT_EQ(Chess::Core::PieceTakeLocator::Find(pawn, pieces, { .file = 'D', .rank = 6 }), (Chess::Core::Coordinate{ .file = 'D', .rank = 5 }));
    }
} // namespace ServerTests
