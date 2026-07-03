module;
#include <array>
#include <cstdint>
#include <string>
#include <boost/asio.hpp>
export module Chess.Net.Connection;
import Chess.Net.Framing;

namespace Chess::Net
{
    export class Connection
    {
        boost::asio::ip::tcp::socket m_socket;

    public:
        explicit Connection(boost::asio::ip::tcp::socket&& socket)
            : m_socket(std::move(socket))
        {
        }

        void SendBytes(const std::string& payload)
        {
            const auto frame = Framing::Encode(payload);
            boost::asio::write(m_socket, boost::asio::buffer(frame));
        }

        std::string ReceiveBytes()
        {
            std::array<uint8_t, Framing::HEADER_SIZE> header{};
            boost::asio::read(m_socket, boost::asio::buffer(header));

            std::string payload;
            payload.resize(Framing::DecodeLength(header));
            boost::asio::read(m_socket, boost::asio::buffer(payload.data(), payload.size()));
            return payload;
        }

        boost::asio::ip::tcp::socket& GetSocket()
        {
            return m_socket;
        }
    };
} // namespace Chess::Net
