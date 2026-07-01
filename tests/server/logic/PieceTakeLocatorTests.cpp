#include <gtest/gtest.h>
#include <memory>
#include <vector>
import Chess.Coordinate;
import Chess.ePieceColor;
import Chess.Pawn;
import Chess.Piece;
import Chess.PieceTakeLocator;

namespace ServerTests
{
    TEST(PieceTakeLocatorTests, ReturnsEnPassantSquareForCapturingPawn)
    {
        // Black pawn double-steps D7 -> D5, becoming en-passant-able.
        const auto blackPawn = std::make_shared<Chess::Pawn>(Chess::ePieceColor::BLACK, Chess::Coordinate{ .file = 'D', .rank = 7 });
        blackPawn->Move({ .file = 'D', .rank = 5 });
        ASSERT_TRUE(blackPawn->GetCanEnPassant());

        const auto whitePawn = std::make_shared<Chess::Pawn>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'E', .rank = 5 });
        const std::vector<std::shared_ptr<Chess::Piece>> pieces = { whitePawn, blackPawn };

        // White pawn captures onto the empty D6 square; the taken piece sits on D5.
        const auto taken = Chess::PieceTakeLocator::Find(whitePawn, pieces, { .file = 'D', .rank = 6 });
        EXPECT_EQ(taken, (Chess::Coordinate{ .file = 'D', .rank = 5 }));
    }

    TEST(PieceTakeLocatorTests, ReturnsTargetSquareForOrdinaryCapture)
    {
        const auto whitePawn = std::make_shared<Chess::Pawn>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'E', .rank = 5 });
        const auto target    = std::make_shared<Chess::Pawn>(Chess::ePieceColor::BLACK, Chess::Coordinate{ .file = 'D', .rank = 6 });
        const std::vector<std::shared_ptr<Chess::Piece>> pieces = { whitePawn, target };

        const auto taken = Chess::PieceTakeLocator::Find(whitePawn, pieces, { .file = 'D', .rank = 6 });
        EXPECT_EQ(taken, (Chess::Coordinate{ .file = 'D', .rank = 6 }));
    }
} // namespace ServerTests
