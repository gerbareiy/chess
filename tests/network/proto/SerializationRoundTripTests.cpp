#include <gtest/gtest.h>
#include <memory>
#include <vector>
import Chess.Core.Coordinate;
import Chess.Core.ePieceColor;
import Chess.Core.ePieceType;
import Chess.Core.King;
import Chess.Core.Move;
import Chess.Core.Pawn;
import Chess.Core.Piece;
import Chess.Core.PieceColorAndType;
import Chess.Network.Chessboard;
import Chess.Network.Coordinate;
import Chess.Network.Move;
import Chess.Network.Piece;
import Chess.Network.PieceColorAndType;

namespace NetworkTests
{
    TEST(CoordinateProtoTests, RoundTrip)
    {
        constexpr Chess::Core::Coordinate coordinate = { .file = 'D', .rank = 4 };
        EXPECT_EQ(Chess::Network::Coordinate::FromProto(Chess::Network::Coordinate::ToProto(coordinate)), coordinate);
    }

    TEST(PieceColorAndTypeProtoTests, ColorRoundTrip)
    {
        for (const auto color : { Chess::Core::ePieceColor::NONE, Chess::Core::ePieceColor::WHITE, Chess::Core::ePieceColor::BLACK })
        {
            EXPECT_EQ(Chess::Network::PieceColorAndType::FromProto(Chess::Network::PieceColorAndType::ToProto(color)), color);
        }
    }

    TEST(PieceColorAndTypeProtoTests, TypeRoundTrip)
    {
        for (const auto type : { Chess::Core::ePieceType::NONE,
                                 Chess::Core::ePieceType::BISHOP,
                                 Chess::Core::ePieceType::KING,
                                 Chess::Core::ePieceType::KNIGHT,
                                 Chess::Core::ePieceType::PAWN,
                                 Chess::Core::ePieceType::QUEEN,
                                 Chess::Core::ePieceType::ROOK })
        {
            EXPECT_EQ(Chess::Network::PieceColorAndType::FromProto(Chess::Network::PieceColorAndType::ToProto(type)), type);
        }
    }

    TEST(PieceColorAndTypeProtoTests, MessageRoundTrip)
    {
        constexpr Chess::Core::PieceColorAndType value = { Chess::Core::ePieceColor::BLACK, Chess::Core::ePieceType::QUEEN };
        EXPECT_EQ(Chess::Network::PieceColorAndType::FromProto(Chess::Network::PieceColorAndType::ToProto(value)), value);
    }

    TEST(MoveProtoTests, RoundTripWithPromotion)
    {
        constexpr Chess::Core::Move move = { .from      = { .file = 'E', .rank = 7 },
                                             .to        = { .file = 'E', .rank = 8 },
                                             .promotion = Chess::Core::ePieceType::QUEEN };
        EXPECT_EQ(Chess::Network::Move::FromProto(Chess::Network::Move::ToProto(move)), move);
    }

    TEST(PieceProtoTests, KingKeepsCastlingFlag)
    {
        const auto king =
            std::make_shared<Chess::Core::King>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'E', .rank = 1 }, false);
        const auto restored = Chess::Network::Piece::FromProto(Chess::Network::Piece::ToProto(king));

        ASSERT_NE(restored, nullptr);
        EXPECT_EQ(restored->GetColorAndType(), king->GetColorAndType());
        EXPECT_EQ(restored->GetPosition(), king->GetPosition());

        const auto restoredKing = std::dynamic_pointer_cast<Chess::Core::King>(restored);
        ASSERT_NE(restoredKing, nullptr);
        EXPECT_FALSE(restoredKing->GetCanMakeCastling());
    }

    TEST(ChessboardProtoTests, PreservesPieceCount)
    {
        const std::vector<std::shared_ptr<Chess::Core::Piece>> pieces = {
            std::make_shared<Chess::Core::King>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'E', .rank = 1 }, false),
            std::make_shared<Chess::Core::King>(Chess::Core::ePieceColor::BLACK, Chess::Core::Coordinate{ .file = 'E', .rank = 8 }, false),
            std::make_shared<Chess::Core::Pawn>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'A', .rank = 2 }),
        };

        const auto restored = Chess::Network::Chessboard::FromProto(Chess::Network::Chessboard::ToProto(pieces, Chess::Core::ePieceColor::WHITE));

        EXPECT_EQ(restored.size(), pieces.size());
    }
} // namespace NetworkTests
