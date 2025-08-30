#include <filesystem>
#include <iostream>
import Chess.Game;

int main()
{
    const auto path = std::filesystem::current_path().parent_path().parent_path().parent_path() / "resources" / "chessboard.json";
    try
    {
        const auto chess = Chess::Game(path);
        chess.Play();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what();
    }

    return 0;
}
