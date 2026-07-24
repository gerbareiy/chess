#include <gtest/gtest.h>
#include <memory>
#include <vector>
import Chess.Core.Coordinate;
import Chess.Core.ePieceColor;
import Chess.Core.Pawn;
import Chess.Core.Piece;
import Chess.Core.PieceTakeLocator;

namespace ServerTests
{
    TEST(PieceTakeLocatorTests, ReturnsEnPassantSquareForCapturingPawn)
    {
        const auto blackPawn =
            std::make_shared<Chess::Core::Pawn>(Chess::Core::ePieceColor::BLACK, Chess::Core::Coordinate{ .file = 'D', .rank = 7 });
        blackPawn->Move({ .file = 'D', .rank = 5 });
        ASSERT_TRUE(blackPawn->GetCanEnPassant());

        const auto whitePawn =
            std::make_shared<Chess::Core::Pawn>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'E', .rank = 5 });
        const std::vector<std::shared_ptr<Chess::Core::Piece>> pieces = { whitePawn, blackPawn };

        const auto taken = Chess::Core::PieceTakeLocator::Find(whitePawn, pieces, { .file = 'D', .rank = 6 });
        EXPECT_EQ(taken, (Chess::Core::Coordinate{ .file = 'D', .rank = 5 }));
    }

    TEST(PieceTakeLocatorTests, ReturnsTargetSquareForOrdinaryCapture)
    {
        const auto whitePawn =
            std::make_shared<Chess::Core::Pawn>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'E', .rank = 5 });
        const auto target = std::make_shared<Chess::Core::Pawn>(Chess::Core::ePieceColor::BLACK, Chess::Core::Coordinate{ .file = 'D', .rank = 6 });
        const std::vector<std::shared_ptr<Chess::Core::Piece>> pieces = { whitePawn, target };

        const auto taken = Chess::Core::PieceTakeLocator::Find(whitePawn, pieces, { .file = 'D', .rank = 6 });
        EXPECT_EQ(taken, (Chess::Core::Coordinate{ .file = 'D', .rank = 6 }));
    }
} // namespace ServerTests
