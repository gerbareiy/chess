#include <gtest/gtest.h>
import Chess.ePieceColor;
import Chess.Pawn;

namespace ServerTests
{
    TEST(PawnTests, TracksFirstMoveAndEnPassantAvailability)
    {
        Chess::Pawn pawn(Chess::ePieceColor::WHITE, { .file = 'E', .rank = 2 });

        EXPECT_TRUE(pawn.GetIsNotMoved());
        EXPECT_FALSE(pawn.GetCanEnPassant());

        pawn.Move({ .file = 'E', .rank = 4 });

        EXPECT_FALSE(pawn.GetIsNotMoved());
        EXPECT_TRUE(pawn.GetCanEnPassant());

        pawn.Move({ .file = 'E', .rank = 5 });

        EXPECT_FALSE(pawn.GetCanEnPassant());
    }
} // namespace ServerTests
