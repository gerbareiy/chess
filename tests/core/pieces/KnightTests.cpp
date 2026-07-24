#include <gtest/gtest.h>
import Chess.Core.Coordinate;
import Chess.Core.ePieceColor;
import Chess.Core.Knight;
import Chess.Utils.Exceptions;

namespace ServerTests
{
    TEST(KnightTests, MovesInLShape)
    {
        auto knight = Chess::Core::Knight(Chess::Core::ePieceColor::WHITE, { .file = 'B', .rank = 1 });
        knight.Move({ .file = 'C', .rank = 3 });
        EXPECT_EQ(knight.GetPosition(), (Chess::Core::Coordinate{ .file = 'C', .rank = 3 }));
    }

    TEST(KnightTests, RejectsStraightAndDiagonalMoves)
    {
        auto knight = Chess::Core::Knight(Chess::Core::ePieceColor::WHITE, { .file = 'B', .rank = 1 });
        EXPECT_THROW(knight.Move({ .file = 'B', .rank = 3 }), Chess::Utils::ImpossibleMoveException);
        EXPECT_THROW(knight.Move({ .file = 'C', .rank = 2 }), Chess::Utils::ImpossibleMoveException);
    }
} // namespace ServerTests
