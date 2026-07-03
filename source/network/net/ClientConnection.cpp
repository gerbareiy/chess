module;
#include <array>
#include <boost/asio.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
module Chess.Net.ClientConnection;
import Chess.Net.Framing;

namespace Chess::Net
{
    struct ClientConnection::Impl
    {
        boost::asio::io_context      io;
        boost::asio::ip::tcp::socket socket{ io };
    };

    ClientConnection::ClientConnection(std::unique_ptr<Impl> impl)
        : m_impl(std::move(impl))
    {
    }

    ClientConnection::ClientConnection(ClientConnection&&) noexcept            = default;
    ClientConnection& ClientConnection::operator=(ClientConnection&&) noexcept = default;
    ClientConnection::~ClientConnection()                                      = default;

    ClientConnection ClientConnection::Connect(const std::string& host, unsigned short port)
    {
        auto impl = std::make_unique<Impl>();

        boost::asio::ip::tcp::resolver resolver(impl->io);
        boost::asio::connect(impl->socket, resolver.resolve(host, std::to_string(port)));

        return ClientConnection(std::move(impl));
    }

    void ClientConnection::SendBytes(const std::string& payload)
    {
        const auto frame = Framing::Encode(payload);
        boost::asio::write(m_impl->socket, boost::asio::buffer(frame));
    }

    std::string ClientConnection::ReceiveBytes()
    {
        std::array<uint8_t, Framing::HEADER_SIZE> header{};
        boost::asio::read(m_impl->socket, boost::asio::buffer(header));

        std::string payload;
        payload.resize(Framing::DecodeLength(header));
        boost::asio::read(m_impl->socket, boost::asio::buffer(payload.data(), payload.size()));
        return payload;
    }

    void ClientConnection::Close()
    {
        boost::system::error_code ignored;
        m_impl->socket.close(ignored);
    }
} // namespace Chess::Net
