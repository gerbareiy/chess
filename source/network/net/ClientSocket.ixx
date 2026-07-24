module;
#include <cstdint>
#include <random>
#include <sstream>
#include <string>
#include <utility>
#include <zmq.hpp>
export module Chess.Net.ClientSocket;
import Chess.Net.ConnectionError;

namespace Chess::Net
{
    // Wraps a ZeroMQ DEALER socket: one logical connection to a server.
    // Portable across Windows/Linux/macOS, unlike raw WinSock.
    export class ClientSocket
    {
        zmq::socket_t m_socket;

        static zmq::context_t& Context()
        {
            static zmq::context_t context(1);
            return context;
        }

        static std::string Endpoint(const std::string& host, unsigned short port)
        {
            return "tcp://" + host + ":" + std::to_string(port);
        }

        // ZeroMQ ROUTER sockets route replies by identity. Assigning each DEALER an
        // explicit, process-unique identity (rather than relying on libzmq's
        // auto-generated one) keeps routing stable for the lifetime of the connection.
        static std::string GenerateIdentity()
        {
            static std::mt19937_64                       engine(std::random_device{}());
            std::uniform_int_distribution<std::uint64_t> distribution;

            std::ostringstream stream;
            stream << std::hex << distribution(engine);
            return stream.str();
        }

        explicit ClientSocket(zmq::socket_t socket)
            : m_socket(std::move(socket))
        {
        }

    public:
        static ClientSocket Connect(const std::string& host, unsigned short port)
        {
            zmq::socket_t socket(Context(), zmq::socket_type::dealer);
            try
            {
                socket.set(zmq::sockopt::routing_id, GenerateIdentity());
                socket.connect(Endpoint(host, port));
            }
            catch (const zmq::error_t& error)
            {
                throw ConnectionError(error.what());
            }
            return ClientSocket(std::move(socket));
        }

        void SendBytes(const std::string& payload)
        {
            try
            {
                m_socket.send(zmq::buffer(payload), zmq::send_flags::none);
            }
            catch (const zmq::error_t& error)
            {
                throw ConnectionError(error.what());
            }
        }

        std::string ReceiveBytes()
        {
            zmq::message_t message;
            try
            {
                if (!m_socket.recv(message))
                {
                    throw ConnectionError("recv failed");
                }
            }
            catch (const zmq::error_t& error)
            {
                throw ConnectionError(error.what());
            }
            return message.to_string();
        }

        void Close()
        {
            m_socket.close();
        }
    };
} // namespace Chess::Net
