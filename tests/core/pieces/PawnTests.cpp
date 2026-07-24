#include <gtest/gtest.h>
import Chess.Core.Coordinate;
import Chess.Core.ePieceColor;
import Chess.Core.Pawn;
import Chess.Utils.Exceptions;

namespace ServerTests
{
    TEST(PawnTests, TracksFirstMoveAndEnPassantAvailability)
    {
        Chess::Core::Pawn pawn(Chess::Core::ePieceColor::WHITE, { .file = 'E', .rank = 2 });

        EXPECT_TRUE(pawn.GetIsNotMoved());
        EXPECT_FALSE(pawn.GetCanEnPassant());

        pawn.Move({ .file = 'E', .rank = 4 });

        EXPECT_FALSE(pawn.GetIsNotMoved());
        EXPECT_TRUE(pawn.GetCanEnPassant());

        pawn.Move({ .file = 'E', .rank = 5 });

        EXPECT_FALSE(pawn.GetCanEnPassant());
    }

    TEST(PawnTests, AllowsForwardAndDiagonalMovesByColor)
    {
        auto whitePawn = Chess::Core::Pawn(Chess::Core::ePieceColor::WHITE, { .file = 'E', .rank = 2 });
        auto blackPawn = Chess::Core::Pawn(Chess::Core::ePieceColor::BLACK, { .file = 'D', .rank = 7 });

        whitePawn.Move({ .file = 'F', .rank = 3 });
        blackPawn.Move({ .file = 'C', .rank = 6 });

        EXPECT_EQ(whitePawn.GetPosition(), (Chess::Core::Coordinate{ .file = 'F', .rank = 3 }));
        EXPECT_EQ(blackPawn.GetPosition(), (Chess::Core::Coordinate{ .file = 'C', .rank = 6 }));
    }

    TEST(PawnTests, RejectsBackwardSidewaysAndLateDoubleMoves)
    {
        auto pawn = Chess::Core::Pawn(Chess::Core::ePieceColor::WHITE, { .file = 'E', .rank = 2 });

        EXPECT_THROW(pawn.Move({ .file = 'E', .rank = 1 }), Chess::Utils::ImpossibleMoveException);
        EXPECT_THROW(pawn.Move({ .file = 'F', .rank = 2 }), Chess::Utils::ImpossibleMoveException);

        pawn.Move({ .file = 'E', .rank = 3 });

        EXPECT_THROW(pawn.Move({ .file = 'E', .rank = 5 }), Chess::Utils::ImpossibleMoveException);
    }
} // namespace ServerTests
