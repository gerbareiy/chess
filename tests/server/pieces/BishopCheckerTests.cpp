#include <algorithm>
#include <gtest/gtest.h>
#include <memory>
#include <vector>
import Chess.Bishop;
import Chess.BishopChecker;
import Chess.Coordinate;
import Chess.ePieceColor;
import Chess.Pawn;
import Chess.Piece;
import Chess.Utils.Exceptions;

namespace ServerTests
{
    namespace
    {
        bool Contains(const std::vector<Chess::Coordinate>& coordinates, const Chess::Coordinate& coordinate)
        {
            return std::ranges::contains(coordinates, coordinate);
        }
    } // namespace

    TEST(BishopCheckerTests, StopsAtOwnPieceAndIncludesOpponentPiece)
    {
        const auto bishop   = std::make_shared<Chess::Bishop>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'D', .rank = 4 });
        const auto ownPiece = std::make_shared<Chess::Pawn>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'F', .rank = 6 });
        const auto opponent = std::make_shared<Chess::Pawn>(Chess::ePieceColor::BLACK, Chess::Coordinate{ .file = 'B', .rank = 2 });
        const std::vector<std::shared_ptr<Chess::Piece>> pieces = { bishop, ownPiece, opponent };

        const auto moves = Chess::BishopChecker(bishop).GetMoves(pieces);

        EXPECT_TRUE(Contains(moves, { .file = 'E', .rank = 5 }));
        EXPECT_FALSE(Contains(moves, { .file = 'F', .rank = 6 }));
        EXPECT_FALSE(Contains(moves, { .file = 'G', .rank = 7 }));

        EXPECT_TRUE(Contains(moves, { .file = 'C', .rank = 3 }));
        EXPECT_TRUE(Contains(moves, { .file = 'B', .rank = 2 }));
        EXPECT_FALSE(Contains(moves, { .file = 'A', .rank = 1 }));

        EXPECT_FALSE(Contains(moves, { .file = 'D', .rank = 5 }));
    }

    TEST(BishopCheckerTests, ThrowsOnNullptrBishop)
    {
        const auto checker = Chess::BishopChecker(nullptr);
        EXPECT_THROW(checker.GetMoves({}), Chess::Utils::PieceIsNullptrException);
    }
} // namespace ServerTests
