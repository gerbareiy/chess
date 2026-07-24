#include <exception>
#include <print>
import Chess.Client.Session;
import Console.Chess.Game;

int main()
{
    try
    {
        auto session = Chess::Client::Session::Connect("127.0.0.1", 5555);
        Console::Chess::Game::RunNetworkGame(session);
    }
    catch (const std::exception& exception)
    {
        std::println("{}", exception.what());
        return 1;
    }
    return 0;
}
