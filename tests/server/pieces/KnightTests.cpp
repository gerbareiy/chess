#include <gtest/gtest.h>
import Chess.Coordinate;
import Chess.ePieceColor;
import Chess.Knight;
import Chess.Utils.Exceptions;

namespace ServerTests
{
    TEST(KnightTests, MovesInLShape)
    {
        auto knight = Chess::Knight(Chess::ePieceColor::WHITE, { .file = 'B', .rank = 1 });
        knight.Move({ .file = 'C', .rank = 3 });
        EXPECT_EQ(knight.GetPosition(), (Chess::Coordinate{ .file = 'C', .rank = 3 }));
    }

    TEST(KnightTests, RejectsStraightAndDiagonalMoves)
    {
        auto knight = Chess::Knight(Chess::ePieceColor::WHITE, { .file = 'B', .rank = 1 });
        EXPECT_THROW(knight.Move({ .file = 'B', .rank = 3 }), Chess::Utils::ImpossibleMoveException);
        EXPECT_THROW(knight.Move({ .file = 'C', .rank = 2 }), Chess::Utils::ImpossibleMoveException);
    }
} // namespace ServerTests
