module;
#include <string>
#include <boost/asio.hpp>
#include "Envelope.pb.h"
export module Chess.Client.MoveClient;
import Chess.Move;
import Chess.Net.Connection;
import Chess.Proto.Envelope;
import Chess.Proto.Move;

namespace Chess::Client
{
    export class MoveClient
    {
    public:
        static Chess::Move SendMove(const std::string& host, unsigned short port, const Chess::Move& move)
        {
            boost::asio::io_context        io;
            boost::asio::ip::tcp::resolver resolver(io);
            boost::asio::ip::tcp::socket   socket(io);
            boost::asio::connect(socket, resolver.resolve(host, std::to_string(port)));

            Chess::Net::Connection connection(std::move(socket));
            connection.SendBytes(Chess::Proto::Envelope::ToProto(move).SerializeAsString());

            chess::proto::Envelope reply;
            reply.ParseFromString(connection.ReceiveBytes());
            return Chess::Proto::Move::FromProto(reply.move());
        }
    };
} // namespace Chess::Client
