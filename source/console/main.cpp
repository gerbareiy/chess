#include <charconv>
#include <exception>
#include <print>
#include <string>
#include <string_view>
import Chess.Client.Session;
import Chess.Console.Game;
import Chess.Constants.Network;

int main(int argc, char* argv[])
{
    auto host = std::string(Chess::Constants::Network::DEFAULT_HOST);
    auto port = Chess::Constants::Network::DEFAULT_PORT;

    if (argc > 1)
    {
        host = argv[1];
    }

    if (argc > 2)
    {
        const std::string_view argument = argv[2];
        unsigned short         parsed   = 0;
        const auto [end, error]         = std::from_chars(argument.data(), argument.data() + argument.size(), parsed);
        if (error != std::errc{} || end != argument.data() + argument.size() || parsed == 0)
        {
            std::println("Usage: console [host] [port]. '{}' is not a valid port.", argument);
            return 1;
        }
        port = parsed;
    }

    try
    {
        auto session = Chess::Client::Session::Connect(host, port);
        Chess::Console::Game::RunNetworkGame(session);
    }
    catch (const std::exception& exception)
    {
        std::println("{}", exception.what());
        return 1;
    }
    return 0;
}
