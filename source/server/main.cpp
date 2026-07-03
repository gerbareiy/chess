#include <exception>
#include <filesystem>
#include <print>
#include <utility>
import Chess.ChessboardBuilder;
import Chess.Net.GameHost;

// Авторитетный сервер: запускается двойным щелчком, ждёт двух игроков и хостит партии одну за другой.
int main()
{
    constexpr unsigned short port = 5555;

    try
    {
        const auto path = std::filesystem::current_path().parent_path().parent_path().parent_path() / "resources" / "chessboard.json";

        std::println("Chess server on port {}. Waiting for two players...", port);
        while (true)
        {
            auto pieces = Chess::ChessboardBuilder::InitBoard(path.string());
            Chess::Net::GameHost::HostSingleMatch(port, std::move(pieces));
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
