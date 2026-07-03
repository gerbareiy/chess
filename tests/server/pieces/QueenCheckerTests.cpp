#include <algorithm>
#include <gtest/gtest.h>
#include <memory>
#include <vector>
import Chess.Coordinate;
import Chess.ePieceColor;
import Chess.Pawn;
import Chess.Piece;
import Chess.Queen;
import Chess.QueenChecker;
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

    TEST(QueenCheckerTests, MovesStraightAndDiagonallyStoppingAtBlockers)
    {
        const auto queen    = std::make_shared<Chess::Queen>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'D', .rank = 4 });
        const auto ownPiece = std::make_shared<Chess::Pawn>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'D', .rank = 6 });
        const auto opponent = std::make_shared<Chess::Pawn>(Chess::ePieceColor::BLACK, Chess::Coordinate{ .file = 'F', .rank = 6 });
        const std::vector<std::shared_ptr<Chess::Piece>> pieces = { queen, ownPiece, opponent };

        const auto moves = Chess::QueenChecker(queen).GetMoves(pieces);

        EXPECT_TRUE(Contains(moves, { .file = 'D', .rank = 5 }));
        EXPECT_FALSE(Contains(moves, { .file = 'D', .rank = 6 }));
        EXPECT_FALSE(Contains(moves, { .file = 'D', .rank = 7 }));

        EXPECT_TRUE(Contains(moves, { .file = 'E', .rank = 5 }));
        EXPECT_TRUE(Contains(moves, { .file = 'F', .rank = 6 }));
        EXPECT_FALSE(Contains(moves, { .file = 'G', .rank = 7 }));

        EXPECT_TRUE(Contains(moves, { .file = 'A', .rank = 4 }));
    }

    TEST(QueenCheckerTests, ThrowsOnNullptrQueen)
    {
        const auto checker = Chess::QueenChecker(nullptr);
        EXPECT_THROW(checker.GetMoves({}), Chess::Utils::PieceIsNullptrException);
    }
} // namespace ServerTests
