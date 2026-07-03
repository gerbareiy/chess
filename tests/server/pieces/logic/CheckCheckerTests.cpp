#include <gtest/gtest.h>
#include <memory>
#include <vector>
import Chess.CheckChecker;
import Chess.Coordinate;
import Chess.ePieceColor;
import Chess.King;
import Chess.Piece;
import Chess.Rook;

namespace ServerTests
{
    TEST(CheckCheckerTests, DetectsCheckAlongOpenFile)
    {
        const auto king = std::make_shared<Chess::King>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'E', .rank = 1 });
        const auto rook = std::make_shared<Chess::Rook>(Chess::ePieceColor::BLACK, Chess::Coordinate{ .file = 'E', .rank = 8 });
        const std::vector<std::shared_ptr<Chess::Piece>> pieces = { king, rook };

        EXPECT_TRUE(Chess::CheckChecker::IsCheck(king, pieces));
        EXPECT_TRUE(Chess::CheckChecker::IsCheck(Chess::ePieceColor::WHITE, pieces));
    }

    TEST(CheckCheckerTests, NoCheckWhenAttackerIsOffLine)
    {
        const auto king = std::make_shared<Chess::King>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'E', .rank = 1 });
        const auto rook = std::make_shared<Chess::Rook>(Chess::ePieceColor::BLACK, Chess::Coordinate{ .file = 'A', .rank = 8 });
        const std::vector<std::shared_ptr<Chess::Piece>> pieces = { king, rook };

        EXPECT_FALSE(Chess::CheckChecker::IsCheck(king, pieces));
        EXPECT_FALSE(Chess::CheckChecker::IsCheck(Chess::ePieceColor::WHITE, pieces));
    }

    TEST(CheckCheckerTests, FriendlyPieceBlocksTheCheck)
    {
        const auto king    = std::make_shared<Chess::King>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'E', .rank = 1 });
        const auto blocker = std::make_shared<Chess::Rook>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'E', .rank = 4 });
        const auto rook    = std::make_shared<Chess::Rook>(Chess::ePieceColor::BLACK, Chess::Coordinate{ .file = 'E', .rank = 8 });
        const std::vector<std::shared_ptr<Chess::Piece>> pieces = { king, blocker, rook };

        EXPECT_FALSE(Chess::CheckChecker::IsCheck(king, pieces));
    }
} // namespace ServerTests
