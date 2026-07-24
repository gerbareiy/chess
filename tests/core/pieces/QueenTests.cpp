#include <gtest/gtest.h>
import Chess.Core.Coordinate;
import Chess.Core.ePieceColor;
import Chess.Core.Queen;
import Chess.Utils.Exceptions;

namespace ServerTests
{
    TEST(QueenTests, MovesStraightAndDiagonally)
    {
        auto queen = Chess::Core::Queen(Chess::Core::ePieceColor::WHITE, { .file = 'D', .rank = 1 });

        queen.Move({ .file = 'D', .rank = 5 });
        EXPECT_EQ(queen.GetPosition(), (Chess::Core::Coordinate{ .file = 'D', .rank = 5 }));

        queen.Move({ .file = 'G', .rank = 8 });
        EXPECT_EQ(queen.GetPosition(), (Chess::Core::Coordinate{ .file = 'G', .rank = 8 }));
    }

    TEST(QueenTests, RejectsLShapedAndEmptyMoves)
    {
        auto queen = Chess::Core::Queen(Chess::Core::ePieceColor::WHITE, { .file = 'D', .rank = 1 });
        EXPECT_THROW(queen.Move({ .file = 'E', .rank = 3 }), Chess::Utils::ImpossibleMoveException);
        EXPECT_THROW(queen.Move({ .file = 'D', .rank = 1 }), Chess::Utils::ImpossibleMoveException);
    }
} // namespace ServerTests
