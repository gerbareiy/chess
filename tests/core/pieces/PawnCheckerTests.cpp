#include <algorithm>
#include <gtest/gtest.h>
#include <memory>
#include <vector>
import Chess.Bishop;
import Chess.Coordinate;
import Chess.ePieceColor;
import Chess.Knight;
import Chess.Pawn;
import Chess.PawnChecker;
import Chess.Piece;
import Chess.PieceTakeLocator;

namespace ServerTests
{
    class PawnCheckerTestHelper
    {
    public:
        static bool Contains(const std::vector<Chess::Coordinate>& coordinates, const Chess::Coordinate& coordinate)
        {
            return std::ranges::contains(coordinates, coordinate);
        }
    };

    TEST(PawnCheckerTests, ReturnsForwardAndCaptureMovesForWhitePawn)
    {
        const auto pawn     = std::make_shared<Chess::Pawn>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'D', .rank = 2 });
        const auto opponent = std::make_shared<Chess::Knight>(Chess::ePieceColor::BLACK, Chess::Coordinate{ .file = 'E', .rank = 3 });
        const auto ownPiece = std::make_shared<Chess::Bishop>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'C', .rank = 3 });
        const std::vector<std::shared_ptr<Chess::Piece>> pieces = { pawn, opponent, ownPiece };

        const auto checker = Chess::PawnChecker(pawn);
        const auto moves   = checker.GetMoves(pieces);

        EXPECT_TRUE(PawnCheckerTestHelper::Contains(moves, { .file = 'D', .rank = 3 }));
        EXPECT_TRUE(PawnCheckerTestHelper::Contains(moves, { .file = 'D', .rank = 4 }));
        EXPECT_TRUE(PawnCheckerTestHelper::Contains(moves, { .file = 'E', .rank = 3 }));
        EXPECT_FALSE(PawnCheckerTestHelper::Contains(moves, { .file = 'C', .rank = 3 }));
    }

    TEST(PawnCheckerTests, SupportsEnPassantCaptureSquare)
    {
        const auto pawn     = std::make_shared<Chess::Pawn>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'E', .rank = 5 });
        const auto opponent = std::make_shared<Chess::Pawn>(Chess::ePieceColor::BLACK, Chess::Coordinate{ .file = 'D', .rank = 7 });
        opponent->Move({ .file = 'D', .rank = 5 });
        const std::vector<std::shared_ptr<Chess::Piece>> pieces = { pawn, opponent };

        const auto checker = Chess::PawnChecker(pawn);
        const auto moves   = checker.GetMoves(pieces);

        EXPECT_TRUE(PawnCheckerTestHelper::Contains(moves, { .file = 'D', .rank = 6 }));
        EXPECT_EQ(Chess::PieceTakeLocator::Find(pawn, pieces, { .file = 'D', .rank = 6 }), (Chess::Coordinate{ .file = 'D', .rank = 5 }));
    }
} // namespace ServerTests
