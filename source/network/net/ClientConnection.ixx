module;
#include <string>
#include <utility>
export module Chess.Net.ClientConnection;
import Chess.Net.ClientSocket;

namespace Chess::Net
{
    export class ClientConnection
    {
    public:
        static ClientConnection Connect(const std::string& host, unsigned short port)
        {
            return ClientConnection(ClientSocket::Connect(host, port));
        }

        void SendBytes(const std::string& payload)
        {
            socket_.SendBytes(payload);
        }

        std::string ReceiveBytes()
        {
            return socket_.ReceiveBytes();
        }

        void Close()
        {
            socket_.Close();
        }

    private:
        ClientSocket socket_;

        explicit ClientConnection(ClientSocket socket)
            : socket_(std::move(socket))
        {
        }
    };
} // namespace Chess::Net
