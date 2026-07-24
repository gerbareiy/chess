#include <gtest/gtest.h>
#include <memory>
#include <vector>
import Chess.Core.CheckChecker;
import Chess.Core.Coordinate;
import Chess.Core.ePieceColor;
import Chess.Core.King;
import Chess.Core.Piece;
import Chess.Core.Rook;

namespace ServerTests
{
    TEST(CheckCheckerTests, DetectsCheckAlongOpenFile)
    {
        const auto king = std::make_shared<Chess::Core::King>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'E', .rank = 1 });
        const auto rook = std::make_shared<Chess::Core::Rook>(Chess::Core::ePieceColor::BLACK, Chess::Core::Coordinate{ .file = 'E', .rank = 8 });
        const std::vector<std::shared_ptr<Chess::Core::Piece>> pieces = { king, rook };

        EXPECT_TRUE(Chess::Core::CheckChecker::IsCheck(king, pieces));
        EXPECT_TRUE(Chess::Core::CheckChecker::IsCheck(Chess::Core::ePieceColor::WHITE, pieces));
    }

    TEST(CheckCheckerTests, NoCheckWhenAttackerIsOffLine)
    {
        const auto king = std::make_shared<Chess::Core::King>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'E', .rank = 1 });
        const auto rook = std::make_shared<Chess::Core::Rook>(Chess::Core::ePieceColor::BLACK, Chess::Core::Coordinate{ .file = 'A', .rank = 8 });
        const std::vector<std::shared_ptr<Chess::Core::Piece>> pieces = { king, rook };

        EXPECT_FALSE(Chess::Core::CheckChecker::IsCheck(king, pieces));
        EXPECT_FALSE(Chess::Core::CheckChecker::IsCheck(Chess::Core::ePieceColor::WHITE, pieces));
    }

    TEST(CheckCheckerTests, FriendlyPieceBlocksTheCheck)
    {
        const auto king    = std::make_shared<Chess::Core::King>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'E', .rank = 1 });
        const auto blocker = std::make_shared<Chess::Core::Rook>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'E', .rank = 4 });
        const auto rook    = std::make_shared<Chess::Core::Rook>(Chess::Core::ePieceColor::BLACK, Chess::Core::Coordinate{ .file = 'E', .rank = 8 });
        const std::vector<std::shared_ptr<Chess::Core::Piece>> pieces = { king, blocker, rook };

        EXPECT_FALSE(Chess::Core::CheckChecker::IsCheck(king, pieces));
    }
} // namespace ServerTests
