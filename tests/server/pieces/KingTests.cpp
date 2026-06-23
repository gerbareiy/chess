#include <gtest/gtest.h>
import Chess.ePieceColor;
import Chess.King;
import Chess.Utils.Exceptions;

namespace ServerTests
{
    TEST(KingTests, DisablesCastlingAfterOrdinaryMoveAndRejectsImpossibleMoves)
    {
        auto king = Chess::King(Chess::ePieceColor::WHITE, { .file = 'E', .rank = 1 });

        EXPECT_TRUE(king.GetCanMakeCastling());

        king.Move({ .file = 'E', .rank = 2 });
        EXPECT_FALSE(king.GetCanMakeCastling());

        EXPECT_THROW(king.Move({ .file = 'E', .rank = 4 }), Chess::Utils::ImpossibleMoveException);
    }

    TEST(KingTests, RejectsTwoFileMoveWhenCastlingIsDisabled)
    {
        auto king = Chess::King(Chess::ePieceColor::WHITE, { .file = 'E', .rank = 1 }, false);
        EXPECT_THROW(king.Move({ .file = 'G', .rank = 1 }), Chess::Utils::ImpossibleMoveException);
    }

    TEST(KingTests, RejectsEmptyMove)
    {
        auto king = Chess::King(Chess::ePieceColor::WHITE, { .file = 'E', .rank = 1 });
        EXPECT_THROW(king.Move({ .file = 'E', .rank = 1 }), Chess::Utils::ImpossibleMoveException);
    }
} // namespace ServerTests
