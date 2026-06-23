#include <gtest/gtest.h>
import Chess.Coordinate;
import Chess.ePieceColor;
import Chess.Queen;
import Chess.Utils.Exceptions;

namespace ServerTests
{
    TEST(QueenTests, MovesStraightAndDiagonally)
    {
        auto queen = Chess::Queen(Chess::ePieceColor::WHITE, { .file = 'D', .rank = 1 });

        queen.Move({ .file = 'D', .rank = 5 });
        EXPECT_EQ(queen.GetPosition(), (Chess::Coordinate{ .file = 'D', .rank = 5 }));

        queen.Move({ .file = 'G', .rank = 8 });
        EXPECT_EQ(queen.GetPosition(), (Chess::Coordinate{ .file = 'G', .rank = 8 }));
    }

    TEST(QueenTests, RejectsLShapedAndEmptyMoves)
    {
        auto queen = Chess::Queen(Chess::ePieceColor::WHITE, { .file = 'D', .rank = 1 });
        EXPECT_THROW(queen.Move({ .file = 'E', .rank = 3 }), Chess::Utils::ImpossibleMoveException);
        EXPECT_THROW(queen.Move({ .file = 'D', .rank = 1 }), Chess::Utils::ImpossibleMoveException);
    }
} // namespace ServerTests
