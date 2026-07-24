#include <algorithm>
#include <gtest/gtest.h>
#include <memory>
#include <vector>
import Chess.Core.Bishop;
import Chess.Core.Coordinate;
import Chess.Core.ePieceColor;
import Chess.Core.King;
import Chess.Core.KingChecker;
import Chess.Core.Knight;
import Chess.Core.Piece;
import Chess.Core.Rook;

namespace ServerTests
{
    class KingCheckerTestHelper
    {
    public:
        static bool Contains(const std::vector<Chess::Core::Coordinate>& coordinates, const Chess::Core::Coordinate& coordinate)
        {
            return std::ranges::contains(coordinates, coordinate);
        }
    };

    TEST(KingCheckerTests, AddsBothCastlingMovesWhenKingAndRooksHaveClearPaths)
    {
        const auto king = std::make_shared<Chess::Core::King>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'E', .rank = 1 });
        const auto leftRook =
            std::make_shared<Chess::Core::Rook>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'A', .rank = 1 }, king);
        const auto rightRook =
            std::make_shared<Chess::Core::Rook>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'H', .rank = 1 }, king);
        const std::vector<std::shared_ptr<Chess::Core::Piece>> pieces = { king, leftRook, rightRook };

        const auto checker = Chess::Core::KingChecker(king);
        const auto moves   = checker.GetMoves(pieces);

        EXPECT_TRUE(KingCheckerTestHelper::Contains(moves, { .file = 'C', .rank = 1 }));
        EXPECT_TRUE(KingCheckerTestHelper::Contains(moves, { .file = 'G', .rank = 1 }));
    }

    TEST(KingCheckerTests, DoesNotAddCastlingMoveWhenPathIsBlocked)
    {
        const auto king = std::make_shared<Chess::Core::King>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'E', .rank = 1 });
        const auto leftRook =
            std::make_shared<Chess::Core::Rook>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'A', .rank = 1 }, king);
        const auto rightRook =
            std::make_shared<Chess::Core::Rook>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'H', .rank = 1 }, king);
        const auto leftBlock =
            std::make_shared<Chess::Core::Bishop>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'D', .rank = 1 });
        const auto rightBlock =
            std::make_shared<Chess::Core::Knight>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'F', .rank = 1 });
        const std::vector<std::shared_ptr<Chess::Core::Piece>> pieces = { king, leftRook, rightRook, leftBlock, rightBlock };

        const auto checker = Chess::Core::KingChecker(king);
        const auto moves   = checker.GetMoves(pieces);

        EXPECT_FALSE(KingCheckerTestHelper::Contains(moves, { .file = 'C', .rank = 1 }));
        EXPECT_FALSE(KingCheckerTestHelper::Contains(moves, { .file = 'G', .rank = 1 }));
    }

    TEST(KingCheckerTests, DoesNotAddCastlingMoveWhenKingIsInCheck)
    {
        const auto king = std::make_shared<Chess::Core::King>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'E', .rank = 1 });
        const auto leftRook =
            std::make_shared<Chess::Core::Rook>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'A', .rank = 1 }, king);
        const auto rightRook =
            std::make_shared<Chess::Core::Rook>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'H', .rank = 1 }, king);
        king->SetCheck(true);
        const std::vector<std::shared_ptr<Chess::Core::Piece>> pieces = { king, leftRook, rightRook };

        const auto checker = Chess::Core::KingChecker(king);
        const auto moves   = checker.GetMoves(pieces);

        EXPECT_FALSE(KingCheckerTestHelper::Contains(moves, { .file = 'C', .rank = 1 }));
        EXPECT_FALSE(KingCheckerTestHelper::Contains(moves, { .file = 'G', .rank = 1 }));
    }

    TEST(KingCheckerTests, DoesNotAddCastlingMoveAfterKingOrRookMoved)
    {
        const auto king = std::make_shared<Chess::Core::King>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'E', .rank = 1 });
        const auto leftRook =
            std::make_shared<Chess::Core::Rook>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'A', .rank = 1 }, king);
        const auto rightRook =
            std::make_shared<Chess::Core::Rook>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'H', .rank = 1 }, king);
        leftRook->Move({ .file = 'A', .rank = 2 });
        leftRook->Move({ .file = 'A', .rank = 1 });
        king->Move({ .file = 'E', .rank = 2 });
        king->Move({ .file = 'E', .rank = 1 });
        const std::vector<std::shared_ptr<Chess::Core::Piece>> pieces = { king, leftRook, rightRook };

        const auto checker = Chess::Core::KingChecker(king);
        const auto moves   = checker.GetMoves(pieces);

        EXPECT_FALSE(KingCheckerTestHelper::Contains(moves, { .file = 'C', .rank = 1 }));
        EXPECT_FALSE(KingCheckerTestHelper::Contains(moves, { .file = 'G', .rank = 1 }));
    }

    TEST(KingCheckerTests, RequiresRookToHaveTheSameColorForCastling)
    {
        const auto king = std::make_shared<Chess::Core::King>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'E', .rank = 1 });
        const auto leftRook =
            std::make_shared<Chess::Core::Rook>(Chess::Core::ePieceColor::BLACK, Chess::Core::Coordinate{ .file = 'A', .rank = 1 }, king);
        const auto rightRook =
            std::make_shared<Chess::Core::Rook>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'H', .rank = 1 }, king);
        const std::vector<std::shared_ptr<Chess::Core::Piece>> pieces = { king, leftRook, rightRook };

        const auto checker = Chess::Core::KingChecker(king);
        const auto moves   = checker.GetMoves(pieces);

        EXPECT_FALSE(KingCheckerTestHelper::Contains(moves, { .file = 'C', .rank = 1 }));
        EXPECT_TRUE(KingCheckerTestHelper::Contains(moves, { .file = 'G', .rank = 1 }));
    }
} // namespace ServerTests
