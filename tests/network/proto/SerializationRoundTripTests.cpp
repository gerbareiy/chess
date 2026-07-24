#include <gtest/gtest.h>
#include <memory>
#include <vector>
import Chess.Coordinate;
import Chess.ePieceColor;
import Chess.ePieceType;
import Chess.King;
import Chess.Move;
import Chess.Pawn;
import Chess.Piece;
import Chess.PieceColorAndType;
import Chess.Proto.Chessboard;
import Chess.Proto.Coordinate;
import Chess.Proto.Move;
import Chess.Proto.Piece;
import Chess.Proto.PieceColorAndType;

namespace NetworkTests
{
    TEST(CoordinateProtoTests, RoundTrip)
    {
        constexpr Chess::Coordinate coordinate = { .file = 'D', .rank = 4 };
        EXPECT_EQ(Chess::Proto::Coordinate::FromProto(Chess::Proto::Coordinate::ToProto(coordinate)), coordinate);
    }

    TEST(PieceColorAndTypeProtoTests, ColorRoundTrip)
    {
        for (const auto color : { Chess::ePieceColor::NONE, Chess::ePieceColor::WHITE, Chess::ePieceColor::BLACK })
        {
            EXPECT_EQ(Chess::Proto::PieceColorAndType::FromProto(Chess::Proto::PieceColorAndType::ToProto(color)), color);
        }
    }

    TEST(PieceColorAndTypeProtoTests, TypeRoundTrip)
    {
        for (const auto type : { Chess::ePieceType::NONE,
                                 Chess::ePieceType::BISHOP,
                                 Chess::ePieceType::KING,
                                 Chess::ePieceType::KNIGHT,
                                 Chess::ePieceType::PAWN,
                                 Chess::ePieceType::QUEEN,
                                 Chess::ePieceType::ROOK })
        {
            EXPECT_EQ(Chess::Proto::PieceColorAndType::FromProto(Chess::Proto::PieceColorAndType::ToProto(type)), type);
        }
    }

    TEST(PieceColorAndTypeProtoTests, MessageRoundTrip)
    {
        constexpr Chess::PieceColorAndType value = { Chess::ePieceColor::BLACK, Chess::ePieceType::QUEEN };
        EXPECT_EQ(Chess::Proto::PieceColorAndType::FromProto(Chess::Proto::PieceColorAndType::ToProto(value)), value);
    }

    TEST(MoveProtoTests, RoundTripWithPromotion)
    {
        constexpr Chess::Move move = { .from = { .file = 'E', .rank = 7 }, .to = { .file = 'E', .rank = 8 }, .promotion = Chess::ePieceType::QUEEN };
        EXPECT_EQ(Chess::Proto::Move::FromProto(Chess::Proto::Move::ToProto(move)), move);
    }

    TEST(PieceProtoTests, KingKeepsCastlingFlag)
    {
        const auto king     = std::make_shared<Chess::King>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'E', .rank = 1 }, false);
        const auto restored = Chess::Proto::Piece::FromProto(Chess::Proto::Piece::ToProto(king));

        ASSERT_NE(restored, nullptr);
        EXPECT_EQ(restored->GetColorAndType(), king->GetColorAndType());
        EXPECT_EQ(restored->GetPosition(), king->GetPosition());

        const auto restoredKing = std::dynamic_pointer_cast<Chess::King>(restored);
        ASSERT_NE(restoredKing, nullptr);
        EXPECT_FALSE(restoredKing->GetCanMakeCastling());
    }

    TEST(ChessboardProtoTests, PreservesPieceCount)
    {
        const std::vector<std::shared_ptr<Chess::Piece>> pieces = {
            std::make_shared<Chess::King>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'E', .rank = 1 }, false),
            std::make_shared<Chess::King>(Chess::ePieceColor::BLACK, Chess::Coordinate{ .file = 'E', .rank = 8 }, false),
            std::make_shared<Chess::Pawn>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'A', .rank = 2 }),
        };

        const auto restored = Chess::Proto::Chessboard::FromProto(Chess::Proto::Chessboard::ToProto(pieces, Chess::ePieceColor::WHITE));

        EXPECT_EQ(restored.size(), pieces.size());
    }
} // namespace NetworkTests
