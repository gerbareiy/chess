#include <exception>
#include <filesystem>
#include <print>
#include <utility>
import Chess.ChessboardBuilder;
import Chess.Net.GameHost;
import Chess.Net.ServerSocket;

int main()
{
    constexpr unsigned short port = 5555;

    try
    {
        const auto path = std::filesystem::current_path().parent_path().parent_path().parent_path() / "resources" / "chessboard.json";

        auto socket = Chess::Net::ServerSocket::Bind(port);

        std::println("Chess server on port {}. Waiting for two players...", port);
        while (true)
        {
            auto pieces = Chess::ChessboardBuilder::InitBoard(path.string());
            Chess::Net::GameHost::HostSingleMatch(socket, std::move(pieces));
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
