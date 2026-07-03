#include <gtest/gtest.h>
#include <memory>
#include <vector>
import Chess.Bishop;
import Chess.Chessboard;
import Chess.Coordinate;
import Chess.DrawChecker;
import Chess.ePieceColor;
import Chess.King;
import Chess.Knight;
import Chess.MoveValidator;
import Chess.Piece;
import Chess.PieceDirector;
import Chess.Player;
import Chess.Queen;
import Chess.Rook;

namespace ServerTests
{
    namespace
    {
        std::shared_ptr<Chess::King> MakeKing(Chess::ePieceColor color, Chess::Coordinate coordinate)
        {
            return std::make_shared<Chess::King>(color, coordinate, false);
        }

        std::shared_ptr<Chess::Chessboard> MakeBoard(
            std::vector<std::shared_ptr<Chess::Piece>> pieces, Chess::ePieceColor toMove = Chess::ePieceColor::WHITE)
        {
            const auto player    = std::make_shared<Chess::Player>(toMove);
            auto       director  = std::make_unique<Chess::PieceDirector>(pieces, player);
            auto       validator = std::make_unique<Chess::MoveValidator>(pieces, player);
            const auto board     = std::make_shared<Chess::Chessboard>(player, std::move(pieces), std::move(director), std::move(validator));
            board->Init();
            return board;
        }

        bool IsDraw(std::vector<std::shared_ptr<Chess::Piece>> pieces, Chess::ePieceColor toMove = Chess::ePieceColor::WHITE)
        {
            const auto board = MakeBoard(std::move(pieces), toMove);
            return Chess::DrawChecker().IsDraw(board);
        }
    } // namespace

    TEST(DrawCheckerTests, LoneKingsAreInsufficientMaterial)
    {
        EXPECT_TRUE(IsDraw(
            { MakeKing(Chess::ePieceColor::WHITE, { .file = 'E', .rank = 1 }), MakeKing(Chess::ePieceColor::BLACK, { .file = 'E', .rank = 8 }) }));
    }

    TEST(DrawCheckerTests, KingAndKnightVersusKingIsInsufficientMaterial)
    {
        EXPECT_TRUE(IsDraw(
            { MakeKing(Chess::ePieceColor::WHITE, { .file = 'E', .rank = 1 }),
              MakeKing(Chess::ePieceColor::BLACK, { .file = 'E', .rank = 8 }),
              std::make_shared<Chess::Knight>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'C', .rank = 3 }) }));
    }

    TEST(DrawCheckerTests, KingAndBishopVersusKingIsInsufficientMaterial)
    {
        EXPECT_TRUE(IsDraw(
            { MakeKing(Chess::ePieceColor::WHITE, { .file = 'E', .rank = 1 }),
              MakeKing(Chess::ePieceColor::BLACK, { .file = 'E', .rank = 8 }),
              std::make_shared<Chess::Bishop>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'C', .rank = 1 }) }));
    }

    TEST(DrawCheckerTests, BishopsOnSameColorAreInsufficientMaterial)
    {
        EXPECT_TRUE(IsDraw(
            { MakeKing(Chess::ePieceColor::WHITE, { .file = 'E', .rank = 1 }),
              MakeKing(Chess::ePieceColor::BLACK, { .file = 'E', .rank = 8 }),
              std::make_shared<Chess::Bishop>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'C', .rank = 1 }),
              std::make_shared<Chess::Bishop>(Chess::ePieceColor::BLACK, Chess::Coordinate{ .file = 'F', .rank = 4 }) }));
    }

    TEST(DrawCheckerTests, BishopsOnOppositeColorsAreNotInsufficientMaterial)
    {
        EXPECT_FALSE(IsDraw(
            { MakeKing(Chess::ePieceColor::WHITE, { .file = 'E', .rank = 1 }),
              MakeKing(Chess::ePieceColor::BLACK, { .file = 'E', .rank = 8 }),
              std::make_shared<Chess::Bishop>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'C', .rank = 1 }),
              std::make_shared<Chess::Bishop>(Chess::ePieceColor::BLACK, Chess::Coordinate{ .file = 'C', .rank = 4 }) }));
    }

    TEST(DrawCheckerTests, KnightVersusKnightIsNotInsufficientMaterial)
    {
        EXPECT_FALSE(IsDraw(
            { MakeKing(Chess::ePieceColor::WHITE, { .file = 'E', .rank = 1 }),
              MakeKing(Chess::ePieceColor::BLACK, { .file = 'E', .rank = 8 }),
              std::make_shared<Chess::Knight>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'C', .rank = 3 }),
              std::make_shared<Chess::Knight>(Chess::ePieceColor::BLACK, Chess::Coordinate{ .file = 'F', .rank = 6 }) }));
    }

    TEST(DrawCheckerTests, RookKeepsMaterialSufficient)
    {
        EXPECT_FALSE(IsDraw(
            { MakeKing(Chess::ePieceColor::WHITE, { .file = 'E', .rank = 1 }),
              MakeKing(Chess::ePieceColor::BLACK, { .file = 'E', .rank = 8 }),
              std::make_shared<Chess::Rook>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'A', .rank = 1 }) }));
    }

    TEST(DrawCheckerTests, StalemateIsDraw)
    {
        EXPECT_TRUE(IsDraw(
            { MakeKing(Chess::ePieceColor::WHITE, { .file = 'F', .rank = 7 }),
              MakeKing(Chess::ePieceColor::BLACK, { .file = 'H', .rank = 8 }),
              std::make_shared<Chess::Queen>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'G', .rank = 6 }) },
            Chess::ePieceColor::BLACK));
    }
} // namespace ServerTests
