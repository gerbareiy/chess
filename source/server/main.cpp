#include <charconv>
#include <exception>
#include <filesystem>
#include <print>
#include <string_view>
#include <utility>
import Chess.Constants.Network;
import Chess.Core.ChessboardBuilder;
import Chess.Network.GameHost;
import Chess.Network.ServerSocket;

int main(int argc, char* argv[])
{
    auto port = Chess::Constants::Network::DEFAULT_PORT;

    if (argc > 1)
    {
        const std::string_view argument = argv[1];
        unsigned short         parsed   = 0;
        const auto [end, error]         = std::from_chars(argument.data(), argument.data() + argument.size(), parsed);
        if (error != std::errc{} || end != argument.data() + argument.size() || parsed == 0)
        {
            std::println("Usage: server [port]. '{}' is not a valid port.", argument);
            return 1;
        }
        port = parsed;
    }

    try
    {
        const auto path = std::filesystem::current_path().parent_path().parent_path().parent_path() / "resources" / "chessboard.json";

        // Fail before binding if the starting position is unreadable, rather than only when
        // the first pair of players is already waiting for a board.
        Chess::Core::ChessboardBuilder::InitBoard(path.string());

        auto socket = Chess::Network::ServerSocket::Bind(port);
        auto host   = Chess::Network::GameHost(socket, [&path] { return Chess::Core::ChessboardBuilder::InitBoard(path.string()); });

        std::println("Chess server on port {}. Serving any number of games at once.", port);
        host.Run();
    }
    catch (const std::exception& exception)
    {
        std::println("Server error: {}", exception.what());
        return 1;
    }

    return 0;
}
