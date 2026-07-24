module;
#include <string>
#include <utility>
export module Chess.Net.ClientConnection;
import Chess.Net.ClientSocket;

namespace Chess::Net
{
    export class ClientConnection
    {
        ClientSocket m_socket;

        explicit ClientConnection(ClientSocket socket)
            : m_socket(std::move(socket))
        {
        }

    public:
        static ClientConnection Connect(const std::string& host, unsigned short port)
        {
            return ClientConnection(ClientSocket::Connect(host, port));
        }

        void SendBytes(const std::string& payload)
        {
            m_socket.SendBytes(payload);
        }

        std::string ReceiveBytes()
        {
            return m_socket.ReceiveBytes();
        }

        void Close()
        {
            m_socket.Close();
        }
    };
} // namespace Chess::Net
