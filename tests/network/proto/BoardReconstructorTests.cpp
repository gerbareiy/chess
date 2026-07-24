#include <gtest/gtest.h>
#include <memory>
#include <vector>
import Chess.Core.Coordinate;
import Chess.Core.ePieceColor;
import Chess.Core.King;
import Chess.Core.Pawn;
import Chess.Core.Piece;
import Chess.Core.Rook;
import Chess.Network.Chessboard;

namespace NetworkTests
{
    class BoardReconstructorTestHelpers
    {
    public:
        template <typename T>
        static std::shared_ptr<T> FindOfColor(const std::vector<std::shared_ptr<Chess::Core::Piece>>& pieces, Chess::Core::ePieceColor color)
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

        static std::vector<std::shared_ptr<Chess::Core::Piece>> RoundTrip(const std::vector<std::shared_ptr<Chess::Core::Piece>>& pieces)
        {
            return Chess::Network::Chessboard::FromProto(Chess::Network::Chessboard::ToProto(pieces, Chess::Core::ePieceColor::WHITE, 0));
        }
    };

    TEST(BoardReconstructorTests, MovedRookLosesCastlingWhileKingKeepsIt)
    {
        const auto whiteKing =
            std::make_shared<Chess::Core::King>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'E', .rank = 1 }, true);
        const auto blackKing =
            std::make_shared<Chess::Core::King>(Chess::Core::ePieceColor::BLACK, Chess::Core::Coordinate{ .file = 'E', .rank = 8 }, true);
        const auto movedRook =
            std::make_shared<Chess::Core::Rook>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'A', .rank = 1 });
        ASSERT_FALSE(movedRook->GetCanMakeCastling());

        const auto restored = BoardReconstructorTestHelpers::RoundTrip({ whiteKing, blackKing, movedRook });

        const auto restoredKing = BoardReconstructorTestHelpers::FindOfColor<Chess::Core::King>(restored, Chess::Core::ePieceColor::WHITE);
        const auto restoredRook = BoardReconstructorTestHelpers::FindOfColor<Chess::Core::Rook>(restored, Chess::Core::ePieceColor::WHITE);
        ASSERT_NE(restoredKing, nullptr);
        ASSERT_NE(restoredRook, nullptr);

        EXPECT_TRUE(restoredKing->GetCanMakeCastling());
        EXPECT_FALSE(restoredRook->GetCanMakeCastling());
    }

    TEST(BoardReconstructorTests, EligibleRookKeepsCastling)
    {
        const auto whiteKing =
            std::make_shared<Chess::Core::King>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'E', .rank = 1 }, true);
        const auto blackKing =
            std::make_shared<Chess::Core::King>(Chess::Core::ePieceColor::BLACK, Chess::Core::Coordinate{ .file = 'E', .rank = 8 }, true);
        const auto rook =
            std::make_shared<Chess::Core::Rook>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'A', .rank = 1 }, whiteKing);
        ASSERT_TRUE(rook->GetCanMakeCastling());

        const auto restored     = BoardReconstructorTestHelpers::RoundTrip({ whiteKing, blackKing, rook });
        const auto restoredRook = BoardReconstructorTestHelpers::FindOfColor<Chess::Core::Rook>(restored, Chess::Core::ePieceColor::WHITE);
        ASSERT_NE(restoredRook, nullptr);
        EXPECT_TRUE(restoredRook->GetCanMakeCastling());
    }

    TEST(BoardReconstructorTests, PawnEnPassantStateRestored)
    {
        const auto whiteKing =
            std::make_shared<Chess::Core::King>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'E', .rank = 1 }, false);
        const auto blackKing =
            std::make_shared<Chess::Core::King>(Chess::Core::ePieceColor::BLACK, Chess::Core::Coordinate{ .file = 'E', .rank = 8 }, false);
        const auto pawn = std::make_shared<Chess::Core::Pawn>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'D', .rank = 2 });
        pawn->Move({ .file = 'D', .rank = 4 });
        ASSERT_TRUE(pawn->GetCanEnPassant());

        const auto restored     = BoardReconstructorTestHelpers::RoundTrip({ whiteKing, blackKing, pawn });
        const auto restoredPawn = BoardReconstructorTestHelpers::FindOfColor<Chess::Core::Pawn>(restored, Chess::Core::ePieceColor::WHITE);
        ASSERT_NE(restoredPawn, nullptr);
        EXPECT_TRUE(restoredPawn->GetCanEnPassant());
        EXPECT_FALSE(restoredPawn->GetIsNotMoved());
    }
} // namespace NetworkTests
