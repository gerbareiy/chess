#include "Envelope.pb.h"
#include <chrono>
#include <exception>
#include <gtest/gtest.h>
#include <memory>
#include <thread>
#include <vector>
import Chess.Coordinate;
import Chess.ePieceColor;
import Chess.King;
import Chess.Move;
import Chess.Net.ClientConnection;
import Chess.Net.GameHost;
import Chess.Net.Socket;
import Chess.Pawn;
import Chess.Piece;
import Chess.Proto.Move;
import Chess.Rook;

namespace NetworkTests
{
    namespace
    {
        void SendFindGame(Chess::Net::ClientConnection& connection)
        {
            chess::proto::Envelope envelope;
            envelope.mutable_find_game();
            connection.SendBytes(envelope.SerializeAsString());
        }

        chess::proto::Envelope Receive(Chess::Net::ClientConnection& connection)
        {
            chess::proto::Envelope envelope;
            envelope.ParseFromString(connection.ReceiveBytes());
            return envelope;
        }

        void SendMove(Chess::Net::ClientConnection& connection, const Chess::Move& move)
        {
            chess::proto::Envelope envelope;
            *envelope.mutable_move() = Chess::Proto::Move::ToProto(move);
            connection.SendBytes(envelope.SerializeAsString());
        }
    } // namespace

    TEST(GameHostTests, RelaysLegalMoveAndRejectsIllegalMove)
    {
        constexpr unsigned short port = 5599;

        std::vector<std::shared_ptr<Chess::Piece>> pieces = {
            std::make_shared<Chess::King>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'E', .rank = 1 }, false),
            std::make_shared<Chess::King>(Chess::ePieceColor::BLACK, Chess::Coordinate{ .file = 'E', .rank = 8 }, false),
            std::make_shared<Chess::Rook>(Chess::ePieceColor::WHITE, Chess::Coordinate{ .file = 'H', .rank = 1 }),
            std::make_shared<Chess::Pawn>(Chess::ePieceColor::BLACK, Chess::Coordinate{ .file = 'A', .rank = 7 }),
        };

        Chess::Net::ServerSocket socket(port);
        auto                     host = std::thread([&socket, pieces]() mutable { Chess::Net::GameHost::HostSingleMatch(socket, std::move(pieces)); });

        auto white = Chess::Net::ClientConnection::Connect("127.0.0.1", port);
        auto black = Chess::Net::ClientConnection::Connect("127.0.0.1", port);

        SendFindGame(white);
        SendFindGame(black);

        const auto whiteStart = Receive(white);
        const auto blackStart = Receive(black);
        EXPECT_EQ(whiteStart.payload_case(), chess::proto::Envelope::kGameStarted);
        EXPECT_EQ(blackStart.payload_case(), chess::proto::Envelope::kGameStarted);
        EXPECT_EQ(whiteStart.game_started().your_color(), chess::proto::PIECE_COLOR_WHITE);
        EXPECT_EQ(blackStart.game_started().your_color(), chess::proto::PIECE_COLOR_BLACK);

        SendMove(white, Chess::Move{ .from = { .file = 'H', .rank = 1 }, .to = { .file = 'H', .rank = 5 } });
        const auto relayed = Receive(black);
        EXPECT_EQ(relayed.payload_case(), chess::proto::Envelope::kMove);
        EXPECT_EQ(Chess::Proto::Move::FromProto(relayed.move()).to, (Chess::Coordinate{ .file = 'H', .rank = 5 }));

        SendMove(black, Chess::Move{ .from = { .file = 'E', .rank = 8 }, .to = { .file = 'E', .rank = 5 } });
        const auto rejected = Receive(black);
        EXPECT_EQ(rejected.payload_case(), chess::proto::Envelope::kBoardSync);

        white.Close();
        black.Close();
        host.join();
    }
} // namespace NetworkTests
