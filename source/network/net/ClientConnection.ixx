module;
#include <memory>
#include <string>
export module Chess.Net.ClientConnection;

namespace Chess::Net
{
    export class ClientConnection
    {
        struct Impl;
        std::unique_ptr<Impl> m_impl;

        explicit ClientConnection(std::unique_ptr<Impl> impl);

    public:
        ClientConnection(ClientConnection&&) noexcept;
        ClientConnection& operator=(ClientConnection&&) noexcept;
        ~ClientConnection();

        static ClientConnection Connect(const std::string& host, unsigned short port);

        void        SendBytes(const std::string& payload);
        std::string ReceiveBytes();
        void        Close();
    };
} // namespace Chess::Net
