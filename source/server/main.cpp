#include <cstdint>
#include <exception>
#include <print>
#include <boost/asio.hpp>
#include "Envelope.pb.h"
import Chess.Move;
import Chess.Net.Connection;
import Chess.Proto.Move;

// Фундаментный сервер: принимает подключение, читает один ход и отправляет его эхом (ack).
// Авторитетная валидация на core-доске и матчмейкинг двух клиентов появятся в фазе B.
int main()
{
    using boost::asio::ip::tcp;
    constexpr unsigned short port = 5555;

    try
    {
        boost::asio::io_context io;
        tcp::acceptor           acceptor(io, tcp::endpoint(tcp::v4(), port));
        std::println("Chess server listening on port {}", port);

        while (true)
        {
            Chess::Net::Connection connection(acceptor.accept());

            chess::proto::Envelope envelope;
            envelope.ParseFromString(connection.ReceiveBytes());

            if (envelope.payload_case() == chess::proto::Envelope::kMove)
            {
                const Chess::Move move = Chess::Proto::Move::FromProto(envelope.move());
                std::println("Received move: {}{} -> {}{}",
                             static_cast<char>(move.from.file),
                             move.from.rank,
                             static_cast<char>(move.to.file),
                             move.to.rank);

                connection.SendBytes(envelope.SerializeAsString());
            }
        }
    }
    catch (const std::exception& exception)
    {
        std::println("Server error: {}", exception.what());
        return 1;
    }

    return 0;
}
