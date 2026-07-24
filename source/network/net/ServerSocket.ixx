module;
#include <string>
#include <utility>
#include <zmq.hpp>
export module Chess.Net.ServerSocket;
import Chess.Net.ConnectionError;
import Chess.Net.Frame;

namespace Chess::Net
{
    // Wraps a ZeroMQ ROUTER socket bound to all interfaces: accepts many
    // clients on a single port and addresses each reply by identity, so the
    // server never touches an OS socket API directly.
    export class ServerSocket
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

        explicit ServerSocket(zmq::socket_t socket)
            : m_socket(std::move(socket))
        {
        }

    public:
        static ServerSocket Bind(unsigned short port)
        {
            zmq::socket_t socket(Context(), zmq::socket_type::router);
            try
            {
                // Peer disconnects don't otherwise unblock a pending recv() on a ROUTER
                // socket, so ask libzmq to surface them as empty-payload frames instead.
                socket.set(zmq::sockopt::router_notify, ZMQ_NOTIFY_DISCONNECT);
                socket.bind(Endpoint("*", port));
            }
            catch (const zmq::error_t& error)
            {
                throw ConnectionError(error.what());
            }
            return ServerSocket(std::move(socket));
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
