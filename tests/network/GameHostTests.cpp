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
import Chess.Pawn;
import Chess.Piece;
import Chess.Proto.Move;
import Chess.Rook;

namespace NetworkTests
{
    namespace
    {
        Chess::Net::ClientConnection ConnectWithRetry(unsigned short port)
        {
            for (int attempt = 0; attempt < 100; ++attempt)
            {
                try
                {
                    return Chess::Net::ClientConnection::Connect("127.0.0.1", port);
                }
                catch (const std::exception&)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(20));
                }
            }
            throw std::runtime_error("could not connect to game host");
        }

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

        std::thread host([pieces]() mutable { Chess::Net::GameHost::HostSingleMatch(port, std::move(pieces)); });

        Chess::Net::ClientConnection white = ConnectWithRetry(port);
        Chess::Net::ClientConnection black = ConnectWithRetry(port);

        SendFindGame(white);
        SendFindGame(black);

        const auto whiteStart = Receive(white);
        const auto blackStart = Receive(black);
        EXPECT_EQ(whiteStart.payload_case(), chess::proto::Envelope::kGameStarted);
        EXPECT_EQ(blackStart.payload_case(), chess::proto::Envelope::kGameStarted);
        EXPECT_EQ(whiteStart.game_started().your_color(), chess::proto::PIECE_COLOR_WHITE);
        EXPECT_EQ(blackStart.game_started().your_color(), chess::proto::PIECE_COLOR_BLACK);

        // Белые делают легальный ход ладьёй H1-H5 — сервер ретранслирует его чёрным.
        SendMove(white, Chess::Move{ .from = { .file = 'H', .rank = 1 }, .to = { .file = 'H', .rank = 5 } });
        const auto relayed = Receive(black);
        EXPECT_EQ(relayed.payload_case(), chess::proto::Envelope::kMove);
        EXPECT_EQ(Chess::Proto::Move::FromProto(relayed.move()).to, (Chess::Coordinate{ .file = 'H', .rank = 5 }));

        // Чёрные пытаются нелегальный ход королём E8-E5 — сервер отклоняет ресинком доски.
        SendMove(black, Chess::Move{ .from = { .file = 'E', .rank = 8 }, .to = { .file = 'E', .rank = 5 } });
        const auto rejected = Receive(black);
        EXPECT_EQ(rejected.payload_case(), chess::proto::Envelope::kBoardSync);

        white.Close();
        black.Close();
        host.join();
    }
} // namespace NetworkTests
