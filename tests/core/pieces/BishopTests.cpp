#include <gtest/gtest.h>
import Chess.Core.Bishop;
import Chess.Core.Coordinate;
import Chess.Core.ePieceColor;
import Chess.Utils.Exceptions;

namespace ServerTests
{
    TEST(BishopTests, MovesDiagonally)
    {
        auto bishop = Chess::Core::Bishop(Chess::Core::ePieceColor::WHITE, { .file = 'C', .rank = 1 });
        bishop.Move({ .file = 'F', .rank = 4 });
        EXPECT_EQ(bishop.GetPosition(), (Chess::Core::Coordinate{ .file = 'F', .rank = 4 }));
    }

    TEST(BishopTests, RejectsStraightAndLShapedMoves)
    {
        auto bishop = Chess::Core::Bishop(Chess::Core::ePieceColor::WHITE, { .file = 'C', .rank = 1 });
        EXPECT_THROW(bishop.Move({ .file = 'C', .rank = 4 }), Chess::Utils::ImpossibleMoveException);
        EXPECT_THROW(bishop.Move({ .file = 'D', .rank = 3 }), Chess::Utils::ImpossibleMoveException);
    }
} // namespace ServerTests
