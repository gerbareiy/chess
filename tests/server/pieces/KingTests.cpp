#include <gtest/gtest.h>
#include <stdexcept>
import Chess.ePieceColor;
import Chess.King;

namespace ServerTests
{
    TEST(KingTests, DisablesCastlingAfterOrdinaryMoveAndRejectsImpossibleMoves)
    {
        Chess::King king(Chess::ePieceColor::WHITE, { .file = 'E', .rank = 1 });

        EXPECT_TRUE(king.GetCanMakeCastling());
        king.Move({ .file = 'E', .rank = 2 });
        EXPECT_FALSE(king.GetCanMakeCastling());
        EXPECT_THROW(king.Move({ .file = 'E', .rank = 4 }), std::logic_error);
    }
} // namespace ServerTests
