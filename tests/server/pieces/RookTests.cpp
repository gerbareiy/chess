#include <gtest/gtest.h>
#include <stdexcept>
import Chess.Coordinate;
import Chess.ePieceColor;
import Chess.Rook;
import Chess.Utils.Exceptions;

namespace ServerTests
{
    TEST(RookTests, MovesStraight)
    {
        auto rook = Chess::Rook(Chess::ePieceColor::WHITE, { .file = 'A', .rank = 1 });

        rook.Move({ .file = 'A', .rank = 8 });
        EXPECT_EQ(rook.GetPosition(), (Chess::Coordinate{ .file = 'A', .rank = 8 }));

        rook.Move({ .file = 'H', .rank = 8 });
        EXPECT_EQ(rook.GetPosition(), (Chess::Coordinate{ .file = 'H', .rank = 8 }));
    }

    TEST(RookTests, RejectsDiagonalAndEmptyMoves)
    {
        auto rook = Chess::Rook(Chess::ePieceColor::WHITE, { .file = 'A', .rank = 1 });
        EXPECT_THROW(rook.Move({ .file = 'B', .rank = 2 }), Chess::Utils::ImpossibleMoveException);
        EXPECT_THROW(rook.Move({ .file = 'A', .rank = 1 }), Chess::Utils::ImpossibleMoveException);
    }
} // namespace ServerTests
