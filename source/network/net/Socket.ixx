module;
#include <zmq.hpp>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
export module Chess.Net.Socket;

namespace Chess::Net
{
    export class ConnectionError : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };

    namespace
    {
        zmq::context_t& Context()
        {
            static zmq::context_t context(1);
            return context;
        }

        std::string Endpoint(const std::string& host, unsigned short port)
        {
            return "tcp://" + host + ":" + std::to_string(port);
        }

        // ZeroMQ ROUTER sockets route replies by identity. Assigning each DEALER an
        // explicit, process-unique identity (rather than relying on libzmq's
        // auto-generated one) keeps routing stable for the lifetime of the connection.
        std::string GenerateIdentity()
        {
            static std::mt19937_64                       engine(std::random_device{}());
            std::uniform_int_distribution<std::uint64_t> distribution;

            std::ostringstream stream;
            stream << std::hex << distribution(engine);
            return stream.str();
        }
    } // namespace

    // Wraps a ZeroMQ DEALER socket: one logical connection to a server.
    // Portable across Windows/Linux/macOS, unlike raw WinSock.
    export class ClientSocket
    {
        zmq::socket_t m_socket;

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

    export struct Frame
    {
        std::string identity;
        std::string payload;
        bool        disconnected = false;
    };

    // Wraps a ZeroMQ ROUTER socket bound to all interfaces: accepts many
    // clients on a single port and addresses each reply by identity, so the
    // server never touches an OS socket API directly.
    export class ServerSocket
    {
        zmq::socket_t m_socket;

    public:
        explicit ServerSocket(unsigned short port)
            : m_socket(Context(), zmq::socket_type::router)
        {
            try
            {
                // Peer disconnects don't otherwise unblock a pending recv() on a ROUTER
                // socket, so ask libzmq to surface them as empty-payload frames instead.
                m_socket.set(zmq::sockopt::router_notify, ZMQ_NOTIFY_DISCONNECT);
                m_socket.bind(Endpoint("*", port));
            }
            catch (const zmq::error_t& error)
            {
                throw ConnectionError(error.what());
            }
        }

        ServerSocket(const ServerSocket&)            = delete;
        ServerSocket& operator=(const ServerSocket&) = delete;

        Frame ReceiveFrame()
        {
            zmq::message_t identity;
            zmq::message_t payload;
            try
            {
                if (!m_socket.recv(identity) || !m_socket.recv(payload))
                {
                    throw ConnectionError("recv failed");
                }
            }
            catch (const zmq::error_t& error)
            {
                throw ConnectionError(error.what());
            }
            return Frame{ identity.to_string(), payload.to_string(), payload.size() == 0 };
        }

        void SendFrame(const std::string& identity, const std::string& payload)
        {
            try
            {
                m_socket.send(zmq::buffer(identity), zmq::send_flags::sndmore);
                m_socket.send(zmq::buffer(payload), zmq::send_flags::none);
            }
            catch (const zmq::error_t& error)
            {
                throw ConnectionError(error.what());
            }
        }
    };
} // namespace Chess::Net
