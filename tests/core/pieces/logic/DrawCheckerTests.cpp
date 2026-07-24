#include <gtest/gtest.h>
#include <memory>
#include <vector>
import Chess.Core.Bishop;
import Chess.Core.Chessboard;
import Chess.Core.Coordinate;
import Chess.Core.DrawChecker;
import Chess.Core.ePieceColor;
import Chess.Core.King;
import Chess.Core.Knight;
import Chess.Core.MoveValidator;
import Chess.Core.Piece;
import Chess.Core.PieceDirector;
import Chess.Core.Player;
import Chess.Core.Queen;
import Chess.Core.Rook;

namespace ServerTests
{
    class DrawCheckerTestHelpers
    {
    public:
        static std::shared_ptr<Chess::Core::King> MakeKing(Chess::Core::ePieceColor color, Chess::Core::Coordinate coordinate)
        {
            return std::make_shared<Chess::Core::King>(color, coordinate, false);
        }

        static std::shared_ptr<Chess::Core::Chessboard> MakeBoard(
            std::vector<std::shared_ptr<Chess::Core::Piece>> pieces, Chess::Core::ePieceColor toMove = Chess::Core::ePieceColor::WHITE)
        {
            const auto player    = std::make_shared<Chess::Core::Player>(toMove);
            auto       director  = std::make_unique<Chess::Core::PieceDirector>(pieces, player);
            auto       validator = std::make_unique<Chess::Core::MoveValidator>(pieces, player);
            const auto board     = std::make_shared<Chess::Core::Chessboard>(player, std::move(pieces), std::move(director), std::move(validator));
            board->Init();
            return board;
        }

        static bool IsDraw(std::vector<std::shared_ptr<Chess::Core::Piece>> pieces, Chess::Core::ePieceColor toMove = Chess::Core::ePieceColor::WHITE)
        {
            const auto board = MakeBoard(std::move(pieces), toMove);
            return Chess::Core::DrawChecker().IsDraw(board);
        }
    };

    TEST(DrawCheckerTests, LoneKingsAreInsufficientMaterial)
    {
        EXPECT_TRUE(
            DrawCheckerTestHelpers::IsDraw(
                { DrawCheckerTestHelpers::MakeKing(Chess::Core::ePieceColor::WHITE, { .file = 'E', .rank = 1 }),
                  DrawCheckerTestHelpers::MakeKing(Chess::Core::ePieceColor::BLACK, { .file = 'E', .rank = 8 }) }));
    }

    TEST(DrawCheckerTests, KingAndKnightVersusKingIsInsufficientMaterial)
    {
        EXPECT_TRUE(
            DrawCheckerTestHelpers::IsDraw(
                { DrawCheckerTestHelpers::MakeKing(Chess::Core::ePieceColor::WHITE, { .file = 'E', .rank = 1 }),
                  DrawCheckerTestHelpers::MakeKing(Chess::Core::ePieceColor::BLACK, { .file = 'E', .rank = 8 }),
                  std::make_shared<Chess::Core::Knight>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'C', .rank = 3 }) }));
    }

    TEST(DrawCheckerTests, KingAndBishopVersusKingIsInsufficientMaterial)
    {
        EXPECT_TRUE(
            DrawCheckerTestHelpers::IsDraw(
                { DrawCheckerTestHelpers::MakeKing(Chess::Core::ePieceColor::WHITE, { .file = 'E', .rank = 1 }),
                  DrawCheckerTestHelpers::MakeKing(Chess::Core::ePieceColor::BLACK, { .file = 'E', .rank = 8 }),
                  std::make_shared<Chess::Core::Bishop>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'C', .rank = 1 }) }));
    }

    TEST(DrawCheckerTests, BishopsOnSameColorAreInsufficientMaterial)
    {
        EXPECT_TRUE(
            DrawCheckerTestHelpers::IsDraw(
                { DrawCheckerTestHelpers::MakeKing(Chess::Core::ePieceColor::WHITE, { .file = 'E', .rank = 1 }),
                  DrawCheckerTestHelpers::MakeKing(Chess::Core::ePieceColor::BLACK, { .file = 'E', .rank = 8 }),
                  std::make_shared<Chess::Core::Bishop>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'C', .rank = 1 }),
                  std::make_shared<Chess::Core::Bishop>(Chess::Core::ePieceColor::BLACK, Chess::Core::Coordinate{ .file = 'F', .rank = 4 }) }));
    }

    TEST(DrawCheckerTests, BishopsOnOppositeColorsAreNotInsufficientMaterial)
    {
        EXPECT_FALSE(
            DrawCheckerTestHelpers::IsDraw(
                { DrawCheckerTestHelpers::MakeKing(Chess::Core::ePieceColor::WHITE, { .file = 'E', .rank = 1 }),
                  DrawCheckerTestHelpers::MakeKing(Chess::Core::ePieceColor::BLACK, { .file = 'E', .rank = 8 }),
                  std::make_shared<Chess::Core::Bishop>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'C', .rank = 1 }),
                  std::make_shared<Chess::Core::Bishop>(Chess::Core::ePieceColor::BLACK, Chess::Core::Coordinate{ .file = 'C', .rank = 4 }) }));
    }

    TEST(DrawCheckerTests, KnightVersusKnightIsNotInsufficientMaterial)
    {
        EXPECT_FALSE(
            DrawCheckerTestHelpers::IsDraw(
                { DrawCheckerTestHelpers::MakeKing(Chess::Core::ePieceColor::WHITE, { .file = 'E', .rank = 1 }),
                  DrawCheckerTestHelpers::MakeKing(Chess::Core::ePieceColor::BLACK, { .file = 'E', .rank = 8 }),
                  std::make_shared<Chess::Core::Knight>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'C', .rank = 3 }),
                  std::make_shared<Chess::Core::Knight>(Chess::Core::ePieceColor::BLACK, Chess::Core::Coordinate{ .file = 'F', .rank = 6 }) }));
    }

    TEST(DrawCheckerTests, RookKeepsMaterialSufficient)
    {
        EXPECT_FALSE(
            DrawCheckerTestHelpers::IsDraw(
                { DrawCheckerTestHelpers::MakeKing(Chess::Core::ePieceColor::WHITE, { .file = 'E', .rank = 1 }),
                  DrawCheckerTestHelpers::MakeKing(Chess::Core::ePieceColor::BLACK, { .file = 'E', .rank = 8 }),
                  std::make_shared<Chess::Core::Rook>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'A', .rank = 1 }) }));
    }

    TEST(DrawCheckerTests, StalemateIsDraw)
    {
        EXPECT_TRUE(
            DrawCheckerTestHelpers::IsDraw(
                { DrawCheckerTestHelpers::MakeKing(Chess::Core::ePieceColor::WHITE, { .file = 'F', .rank = 7 }),
                  DrawCheckerTestHelpers::MakeKing(Chess::Core::ePieceColor::BLACK, { .file = 'H', .rank = 8 }),
                  std::make_shared<Chess::Core::Queen>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'G', .rank = 6 }) },
                Chess::Core::ePieceColor::BLACK));
    }
} // namespace ServerTests
