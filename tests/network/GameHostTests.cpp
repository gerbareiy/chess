#include "Envelope.pb.h"
#include <atomic>
#include <chrono>
#include <exception>
#include <gtest/gtest.h>
#include <memory>
#include <random>
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

        // Hardcoded ports made these tests fail with "Address in use" whenever a port was
        // still held by another run, so each case claims its own instead.
        static unsigned short NextPort()
        {
            static std::atomic<unsigned short> offset = static_cast<unsigned short>(std::random_device{}() % 20000);
            return static_cast<unsigned short>(40000 + offset++ % 20000);
        }

        static std::vector<std::shared_ptr<Chess::Core::Piece>> CreateSparsePieces()
        {
            return {
                std::make_shared<Chess::Core::King>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'E', .rank = 1 }, false),
                std::make_shared<Chess::Core::King>(Chess::Core::ePieceColor::BLACK, Chess::Core::Coordinate{ .file = 'E', .rank = 8 }, false),
                std::make_shared<Chess::Core::Rook>(Chess::Core::ePieceColor::WHITE, Chess::Core::Coordinate{ .file = 'H', .rank = 1 }),
                std::make_shared<Chess::Core::Pawn>(Chess::Core::ePieceColor::BLACK, Chess::Core::Coordinate{ .file = 'A', .rank = 7 }),
            };
        }
    };

    TEST(GameHostTests, RelaysLegalMoveAndRejectsIllegalMove)
    {
        const unsigned short port = GameHostTestHelpers::NextPort();

        auto socket = Chess::Network::ServerSocket::Bind(port);
        auto host   = Chess::Network::GameHost(socket, &GameHostTestHelpers::CreateSparsePieces);
        auto thread = std::thread([&host] { host.RunUntilIdle(std::chrono::seconds(2)); });

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
        thread.join();
    }

    TEST(GameHostTests, HostsTwoMatchesAtTheSameTime)
    {
        const unsigned short port = GameHostTestHelpers::NextPort();

        auto socket = Chess::Network::ServerSocket::Bind(port);
        auto host   = Chess::Network::GameHost(socket, &GameHostTestHelpers::CreateSparsePieces);
        auto thread = std::thread([&host] { host.RunUntilIdle(std::chrono::seconds(2)); });

        auto firstWhite  = Chess::Network::ClientConnection::Connect("127.0.0.1", port);
        auto firstBlack  = Chess::Network::ClientConnection::Connect("127.0.0.1", port);
        auto secondWhite = Chess::Network::ClientConnection::Connect("127.0.0.1", port);
        auto secondBlack = Chess::Network::ClientConnection::Connect("127.0.0.1", port);

        GameHostTestHelpers::SendFindGame(firstWhite);
        GameHostTestHelpers::SendFindGame(firstBlack);
        GameHostTestHelpers::SendFindGame(secondWhite);
        GameHostTestHelpers::SendFindGame(secondBlack);

        // All four are given a game, so the second pair is served without waiting for the
        // first match to end.
        EXPECT_EQ(GameHostTestHelpers::Receive(firstWhite).game_started().your_color(), chess::proto::PIECE_COLOR_WHITE);
        EXPECT_EQ(GameHostTestHelpers::Receive(firstBlack).game_started().your_color(), chess::proto::PIECE_COLOR_BLACK);
        EXPECT_EQ(GameHostTestHelpers::Receive(secondWhite).game_started().your_color(), chess::proto::PIECE_COLOR_WHITE);
        EXPECT_EQ(GameHostTestHelpers::Receive(secondBlack).game_started().your_color(), chess::proto::PIECE_COLOR_BLACK);

        // Both matches are live at once: each relays its own moves independently.
        GameHostTestHelpers::SendMove(secondWhite, Chess::Core::Move{ .from = { .file = 'H', .rank = 1 }, .to = { .file = 'H', .rank = 5 } });
        EXPECT_EQ(GameHostTestHelpers::Receive(secondBlack).payload_case(), chess::proto::Envelope::kMove);

        GameHostTestHelpers::SendMove(firstWhite, Chess::Core::Move{ .from = { .file = 'H', .rank = 1 }, .to = { .file = 'H', .rank = 3 } });
        EXPECT_EQ(GameHostTestHelpers::Receive(firstBlack).payload_case(), chess::proto::Envelope::kMove);

        firstWhite.Close();
        firstBlack.Close();
        secondWhite.Close();
        secondBlack.Close();
        thread.join();
    }

    TEST(GameHostTests, TellsTheSurvivorWhenTheOpponentDisconnects)
    {
        const unsigned short port = GameHostTestHelpers::NextPort();

        auto socket = Chess::Network::ServerSocket::Bind(port);
        auto host   = Chess::Network::GameHost(socket, &GameHostTestHelpers::CreateSparsePieces);
        auto thread = std::thread([&host] { host.RunUntilIdle(std::chrono::seconds(2)); });

        auto white = Chess::Network::ClientConnection::Connect("127.0.0.1", port);
        {
            auto black = Chess::Network::ClientConnection::Connect("127.0.0.1", port);

            GameHostTestHelpers::SendFindGame(white);
            GameHostTestHelpers::SendFindGame(black);

            EXPECT_EQ(GameHostTestHelpers::Receive(white).payload_case(), chess::proto::Envelope::kGameStarted);
            EXPECT_EQ(GameHostTestHelpers::Receive(black).payload_case(), chess::proto::Envelope::kGameStarted);

            black.Close();
        }

        // Previously the surviving player was told nothing and blocked here forever.
        EXPECT_EQ(GameHostTestHelpers::Receive(white).payload_case(), chess::proto::Envelope::kOpponentLeft);

        white.Close();
        thread.join();
    }
} // namespace NetworkTests
