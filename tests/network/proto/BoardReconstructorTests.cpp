#include <gtest/gtest.h>
#include <memory>
#include <vector>
import Chess.Coordinate;
import Chess.ePieceColor;
import Chess.King;
import Chess.Pawn;
import Chess.Piece;
import Chess.Proto.Chessboard;
import Chess.Rook;

namespace NetworkTests
{
    namespace
    {
        template <typename T> std::shared_ptr<T> FindOfColor(const std::vector<std::shared_ptr<Chess::Piece>>& pieces, Chess::ePieceColor color)
        {
            for (const auto& piece : pieces)
            {
                if (const auto typed = std::dynamic_pointer_cast<T>(piece); typed && typed->GetColorAndType().color == color)
                {
                    return typed;
                }
            }
            return nullptr;
        }

        std::vector<std::shared_ptr<Chess::Piece>> RoundTrip(const std::vector<std::shared_ptr<Chess::Piece>>& pieces)
        {
            return Chess::Proto::Chessboard::FromProto(Chess::Proto::Chessboard::ToProto(pieces, Chess::ePieceColor::WHITE, 0, 0));
        }
    } // namespace

    TEST(BoardReconstructorTests, MovedRookLosesCastlingWhileKingKeepsIt)
    {
        const auto whiteKing = std::make_shared<Chess::King>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'E', .rank = 1 }, true);
        const auto blackKing = std::make_shared<Chess::King>(Chess::ePieceColor::BLACK, Chess::Coordinate{ .file = 'E', .rank = 8 }, true);
        const auto movedRook = std::make_shared<Chess::Rook>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'A', .rank = 1 });
        ASSERT_FALSE(movedRook->GetCanMakeCastling());

        const auto restored = RoundTrip({ whiteKing, blackKing, movedRook });

        const auto restoredKing = FindOfColor<Chess::King>(restored, Chess::ePieceColor::WHITE);
        const auto restoredRook = FindOfColor<Chess::Rook>(restored, Chess::ePieceColor::WHITE);
        ASSERT_NE(restoredKing, nullptr);
        ASSERT_NE(restoredRook, nullptr);

        EXPECT_TRUE(restoredKing->GetCanMakeCastling());  // король ещё может
        EXPECT_FALSE(restoredRook->GetCanMakeCastling()); // а эта ладья уже нет
    }

    TEST(BoardReconstructorTests, EligibleRookKeepsCastling)
    {
        const auto whiteKing = std::make_shared<Chess::King>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'E', .rank = 1 }, true);
        const auto blackKing = std::make_shared<Chess::King>(Chess::ePieceColor::BLACK, Chess::Coordinate{ .file = 'E', .rank = 8 }, true);
        const auto rook      = std::make_shared<Chess::Rook>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'A', .rank = 1 }, whiteKing);
        ASSERT_TRUE(rook->GetCanMakeCastling());

        const auto restored     = RoundTrip({ whiteKing, blackKing, rook });
        const auto restoredRook = FindOfColor<Chess::Rook>(restored, Chess::ePieceColor::WHITE);
        ASSERT_NE(restoredRook, nullptr);
        EXPECT_TRUE(restoredRook->GetCanMakeCastling());
    }

    TEST(BoardReconstructorTests, PawnEnPassantStateRestored)
    {
        const auto whiteKing = std::make_shared<Chess::King>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'E', .rank = 1 }, false);
        const auto blackKing = std::make_shared<Chess::King>(Chess::ePieceColor::BLACK, Chess::Coordinate{ .file = 'E', .rank = 8 }, false);
        const auto pawn      = std::make_shared<Chess::Pawn>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'D', .rank = 2 });
        pawn->Move({ .file = 'D', .rank = 4 }); // двойной ход — становится доступным для взятия на проходе
        ASSERT_TRUE(pawn->GetCanEnPassant());

        const auto restored     = RoundTrip({ whiteKing, blackKing, pawn });
        const auto restoredPawn = FindOfColor<Chess::Pawn>(restored, Chess::ePieceColor::WHITE);
        ASSERT_NE(restoredPawn, nullptr);
        EXPECT_TRUE(restoredPawn->GetCanEnPassant());
        EXPECT_FALSE(restoredPawn->GetIsNotMoved());
    }
} // namespace NetworkTests
