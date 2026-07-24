#include "Envelope.pb.h"
#include <chrono>
#include <exception>
#include <gtest/gtest.h>
#include <memory>
#include <thread>
#include <vector>
import Chess.Core.Coordinate;
import Chess.Core.ePieceColor;
import Chess.Core.King;
import Chess.Core.Move;
import Chess.Core.Pawn;
import Chess.Core.Piece;
import Chess.Core.Rook;
import Chess.Network.ClientConnection;
import Chess.Network.GameHost;
import Chess.Network.Move;
import Chess.Network.ServerSocket;

namespace NetworkTests
{
    class GameHostTestHelpers
    {
    public:
        static void SendFindGame(Chess::Network::ClientConnection& connection)
        {
            chess::proto::Envelope envelope;
            envelope.mutable_find_game();
            connection.SendBytes(envelope.SerializeAsString());
        }

        static chess::proto::Envelope Receive(Chess::Network::ClientConnection& connection)
        {
            chess::proto::Envelope envelope;
            envelope.ParseFromString(connection.ReceiveBytes());
            return envelope;
        }

        static void SendMove(Chess::Network::ClientConnection& connection, const Chess::Core::Move& move)
        {
            chess::proto::Envelope envelope;
            *envelope.mutable_move() = Chess::Network::Move::ToProto(move);
            connection.SendBytes(envelope.SerializeAsString());
        }
    };

    TEST(GameHostTests, RelaysLegalMoveAndRejectsIllegalMove)
    {
        constexpr unsigned short port = 5599;

        std::vector<std::shared_ptr<Chess::Core::Piece>> pieces = {
            std::make_shared<Chess::Core::King>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'E', .rank = 1 }, false),
            std::make_shared<Chess::Core::King>(Chess::Core::ePieceColor::BLACK, Chess::Core::Coordinate{ .file = 'E', .rank = 8 }, false),
            std::make_shared<Chess::Core::Rook>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'H', .rank = 1 }),
            std::make_shared<Chess::Core::Pawn>(Chess::Core::ePieceColor::BLACK, Chess::Core::Coordinate{ .file = 'A', .rank = 7 }),
        };

        auto socket = Chess::Network::ServerSocket::Bind(port);
        auto host   = std::thread([&socket, pieces]() mutable { Chess::Network::GameHost::HostSingleMatch(socket, std::move(pieces)); });

        auto white = Chess::Network::ClientConnection::Connect("127.0.0.1", port);
        auto black = Chess::Network::ClientConnection::Connect("127.0.0.1", port);

        GameHostTestHelpers::SendFindGame(white);
        GameHostTestHelpers::SendFindGame(black);

        const auto whiteStart = GameHostTestHelpers::Receive(white);
        const auto blackStart = GameHostTestHelpers::Receive(black);
        EXPECT_EQ(whiteStart.payload_case(), chess::proto::Envelope::kGameStarted);
        EXPECT_EQ(blackStart.payload_case(), chess::proto::Envelope::kGameStarted);
        EXPECT_EQ(whiteStart.game_started().your_color(), chess::proto::PIECE_COLOR_WHITE);
        EXPECT_EQ(blackStart.game_started().your_color(), chess::proto::PIECE_COLOR_BLACK);

        GameHostTestHelpers::SendMove(white, Chess::Core::Move{ .from = { .file = 'H', .rank = 1 }, .to = { .file = 'H', .rank = 5 } });
        const auto relayed = GameHostTestHelpers::Receive(black);
        EXPECT_EQ(relayed.payload_case(), chess::proto::Envelope::kMove);
        EXPECT_EQ(Chess::Network::Move::FromProto(relayed.move()).to, (Chess::Core::Coordinate{ .file = 'H', .rank = 5 }));

        GameHostTestHelpers::SendMove(black, Chess::Core::Move{ .from = { .file = 'E', .rank = 8 }, .to = { .file = 'E', .rank = 5 } });
        const auto rejected = GameHostTestHelpers::Receive(black);
        EXPECT_EQ(rejected.payload_case(), chess::proto::Envelope::kBoardSync);

        white.Close();
        black.Close();
        host.join();
    }
} // namespace NetworkTests
