#include <exception>
#include <filesystem>
#include <print>
#include <utility>
import Chess.Core.ChessboardBuilder;
import Chess.Network.GameHost;
import Chess.Network.ServerSocket;

int main()
{
    constexpr unsigned short port = 5555;

    try
    {
        const auto path = std::filesystem::current_path().parent_path().parent_path().parent_path() / "resources" / "chessboard.json";

        auto socket = Chess::Network::ServerSocket::Bind(port);

        std::println("Chess server on port {}. Waiting for two players...", port);
        while (true)
        {
            auto pieces = Chess::Core::ChessboardBuilder::InitBoard(path.string());
            Chess::Network::GameHost::HostSingleMatch(socket, std::move(pieces));
            std::println("Match finished. Waiting for the next two players...");
        }
    }
    catch (const std::exception& exception)
    {
        std::println("Server error: {}", exception.what());
        return 1;
    }

    return 0;
}
