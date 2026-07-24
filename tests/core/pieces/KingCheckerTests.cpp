#include <algorithm>
#include <gtest/gtest.h>
#include <memory>
#include <vector>
import Chess.Bishop;
import Chess.Coordinate;
import Chess.ePieceColor;
import Chess.King;
import Chess.KingChecker;
import Chess.Knight;
import Chess.Piece;
import Chess.Rook;

namespace ServerTests
{
    class KingCheckerTestHelper
    {
    public:
        static bool Contains(const std::vector<Chess::Coordinate>& coordinates, const Chess::Coordinate& coordinate)
        {
            return std::ranges::contains(coordinates, coordinate);
        }
    };

    TEST(KingCheckerTests, AddsBothCastlingMovesWhenKingAndRooksHaveClearPaths)
    {
        const auto king      = std::make_shared<Chess::King>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'E', .rank = 1 });
        const auto leftRook  = std::make_shared<Chess::Rook>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'A', .rank = 1 }, king);
        const auto rightRook = std::make_shared<Chess::Rook>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'H', .rank = 1 }, king);
        const std::vector<std::shared_ptr<Chess::Piece>> pieces = { king, leftRook, rightRook };

        const auto checker = Chess::KingChecker(king);
        const auto moves   = checker.GetMoves(pieces);

        EXPECT_TRUE(KingCheckerTestHelper::Contains(moves, { .file = 'C', .rank = 1 }));
        EXPECT_TRUE(KingCheckerTestHelper::Contains(moves, { .file = 'G', .rank = 1 }));
    }

    TEST(KingCheckerTests, DoesNotAddCastlingMoveWhenPathIsBlocked)
    {
        const auto king       = std::make_shared<Chess::King>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'E', .rank = 1 });
        const auto leftRook   = std::make_shared<Chess::Rook>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'A', .rank = 1 }, king);
        const auto rightRook  = std::make_shared<Chess::Rook>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'H', .rank = 1 }, king);
        const auto leftBlock  = std::make_shared<Chess::Bishop>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'D', .rank = 1 });
        const auto rightBlock = std::make_shared<Chess::Knight>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'F', .rank = 1 });
        const std::vector<std::shared_ptr<Chess::Piece>> pieces = { king, leftRook, rightRook, leftBlock, rightBlock };

        const auto checker = Chess::KingChecker(king);
        const auto moves   = checker.GetMoves(pieces);

        EXPECT_FALSE(KingCheckerTestHelper::Contains(moves, { .file = 'C', .rank = 1 }));
        EXPECT_FALSE(KingCheckerTestHelper::Contains(moves, { .file = 'G', .rank = 1 }));
    }

    TEST(KingCheckerTests, DoesNotAddCastlingMoveWhenKingIsInCheck)
    {
        const auto king      = std::make_shared<Chess::King>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'E', .rank = 1 });
        const auto leftRook  = std::make_shared<Chess::Rook>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'A', .rank = 1 }, king);
        const auto rightRook = std::make_shared<Chess::Rook>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'H', .rank = 1 }, king);
        king->SetCheck(true);
        const std::vector<std::shared_ptr<Chess::Piece>> pieces = { king, leftRook, rightRook };

        const auto checker = Chess::KingChecker(king);
        const auto moves   = checker.GetMoves(pieces);

        EXPECT_FALSE(KingCheckerTestHelper::Contains(moves, { .file = 'C', .rank = 1 }));
        EXPECT_FALSE(KingCheckerTestHelper::Contains(moves, { .file = 'G', .rank = 1 }));
    }

    TEST(KingCheckerTests, DoesNotAddCastlingMoveAfterKingOrRookMoved)
    {
        const auto king      = std::make_shared<Chess::King>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'E', .rank = 1 });
        const auto leftRook  = std::make_shared<Chess::Rook>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'A', .rank = 1 }, king);
        const auto rightRook = std::make_shared<Chess::Rook>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'H', .rank = 1 }, king);
        leftRook->Move({ .file = 'A', .rank = 2 });
        leftRook->Move({ .file = 'A', .rank = 1 });
        king->Move({ .file = 'E', .rank = 2 });
        king->Move({ .file = 'E', .rank = 1 });
        const std::vector<std::shared_ptr<Chess::Piece>> pieces = { king, leftRook, rightRook };

        const auto checker = Chess::KingChecker(king);
        const auto moves   = checker.GetMoves(pieces);

        EXPECT_FALSE(KingCheckerTestHelper::Contains(moves, { .file = 'C', .rank = 1 }));
        EXPECT_FALSE(KingCheckerTestHelper::Contains(moves, { .file = 'G', .rank = 1 }));
    }

    TEST(KingCheckerTests, RequiresRookToHaveTheSameColorForCastling)
    {
        const auto king      = std::make_shared<Chess::King>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'E', .rank = 1 });
        const auto leftRook  = std::make_shared<Chess::Rook>(Chess::ePieceColor::BLACK, Chess::Coordinate{ .file = 'A', .rank = 1 }, king);
        const auto rightRook = std::make_shared<Chess::Rook>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'H', .rank = 1 }, king);
        const std::vector<std::shared_ptr<Chess::Piece>> pieces = { king, leftRook, rightRook };

        const auto checker = Chess::KingChecker(king);
        const auto moves   = checker.GetMoves(pieces);

        EXPECT_FALSE(KingCheckerTestHelper::Contains(moves, { .file = 'C', .rank = 1 }));
        EXPECT_TRUE(KingCheckerTestHelper::Contains(moves, { .file = 'G', .rank = 1 }));
    }
} // namespace ServerTests
