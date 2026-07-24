#include <gtest/gtest.h>
#include <memory>
#include <stdexcept>
import Chess.Core.Coordinate;
import Chess.Core.ePieceColor;
import Chess.Core.King;
import Chess.Core.Rook;
import Chess.Utils.Exceptions;

namespace ServerTests
{
    TEST(RookTests, MovesStraight)
    {
        auto rook = Chess::Core::Rook(Chess::Core::ePieceColor::WHITE, { .file = 'A', .rank = 1 });

        rook.Move({ .file = 'A', .rank = 8 });
        EXPECT_EQ(rook.GetPosition(), (Chess::Core::Coordinate{ .file = 'A', .rank = 8 }));

        rook.Move({ .file = 'H', .rank = 8 });
        EXPECT_EQ(rook.GetPosition(), (Chess::Core::Coordinate{ .file = 'H', .rank = 8 }));
    }

    TEST(RookTests, RejectsDiagonalAndEmptyMoves)
    {
        auto rook = Chess::Core::Rook(Chess::Core::ePieceColor::WHITE, { .file = 'A', .rank = 1 });
        EXPECT_THROW(rook.Move({ .file = 'B', .rank = 2 }), Chess::Utils::ImpossibleMoveException);
        EXPECT_THROW(rook.Move({ .file = 'A', .rank = 1 }), Chess::Utils::ImpossibleMoveException);
    }

    TEST(RookTests, FollowsKingWhenItCastles)
    {
        auto king = std::make_shared<Chess::Core::King>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'E', .rank = 1 });
        auto rook = std::make_shared<Chess::Core::Rook>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'H', .rank = 1 }, king);

        EXPECT_TRUE(rook->GetCanMakeCastling());

        king->Move({ .file = 'G', .rank = 1 });

        EXPECT_EQ(rook->GetPosition(), (Chess::Core::Coordinate{ .file = 'F', .rank = 1 }));
        EXPECT_FALSE(rook->GetCanMakeCastling());
    }

    TEST(RookTests, LosesCastlingRightsWhenKingMovesNormally)
    {
        auto king = std::make_shared<Chess::Core::King>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'E', .rank = 1 });
        auto rook = std::make_shared<Chess::Core::Rook>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'H', .rank = 1 }, king);

        king->Move({ .file = 'E', .rank = 2 });

        EXPECT_FALSE(rook->GetCanMakeCastling());
        EXPECT_EQ(rook->GetPosition(), (Chess::Core::Coordinate{ .file = 'H', .rank = 1 }));
    }

    TEST(RookTests, LosesCastlingRightsAfterOwnMove)
    {
        auto king = std::make_shared<Chess::Core::King>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'E', .rank = 1 });
        auto rook = std::make_shared<Chess::Core::Rook>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'H', .rank = 1 }, king);

        rook->Move({ .file = 'H', .rank = 4 });

        EXPECT_FALSE(rook->GetCanMakeCastling());
    }
} // namespace ServerTests
