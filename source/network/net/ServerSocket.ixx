module;
#include <chrono>
#include <optional>
#include <string>
#include <utility>
#include <zmq.hpp>
export module Chess.Network.ServerSocket;
import Chess.Network.ConnectionError;
import Chess.Network.Frame;

namespace Chess::Network
{
    // Wraps a ZeroMQ ROUTER socket bound to all interfaces: accepts many
    // clients on a single port and addresses each reply by identity, so the
    // server never touches an OS socket API directly.
    export class ServerSocket
    {
    public:
        static ServerSocket Bind(unsigned short port)
        {
            zmq::socket_t socket(Context(), zmq::socket_type::router);
            try
            {
                // Peer disconnects don't otherwise unblock a pending recv() on a ROUTER
                // socket, so ask libzmq to surface them as empty-payload frames instead.
                socket.set(zmq::sockopt::router_notify, ZMQ_NOTIFY_DISCONNECT);

                // libzmq lingers forever by default, so a socket closed while it still holds
                // messages queued for a peer that already went away keeps the port bound -
                // and the next bind on it fails with "Address in use". Undelivered messages
                // are worthless once we are shutting down, so drop them and free the port.
                socket.set(zmq::sockopt::linger, 0);

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
            const auto frame = TryReceiveFrame(std::chrono::milliseconds(-1));
            if (!frame.has_value())
            {
                throw ConnectionError("recv failed");
            }
            return frame.value();
        }

        // A negative timeout blocks indefinitely; otherwise returns nullopt once the
        // timeout expires with nothing to read, so a caller can stop on an idle socket.
        std::optional<Frame> TryReceiveFrame(std::chrono::milliseconds timeout)
        {
            zmq::message_t identity;
            zmq::message_t payload;
            try
            {
                socket_.set(zmq::sockopt::rcvtimeo, static_cast<int>(timeout.count()));

                if (!socket_.recv(identity))
                {
                    return std::nullopt;
                }
                if (!socket_.recv(payload))
                {
                    throw ConnectionError("recv failed after the identity part");
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
                socket_.send(zmq::buffer(identity), zmq::send_flags::sndmore);
                socket_.send(zmq::buffer(payload), zmq::send_flags::none);
            }
            catch (const zmq::error_t& error)
            {
                throw ConnectionError(error.what());
            }
        }

    private:
        zmq::socket_t socket_;

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
            : socket_(std::move(socket))
        {
        }
    };
} // namespace Chess::Network
