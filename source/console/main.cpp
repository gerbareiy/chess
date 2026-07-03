#include <filesystem>
#include <print>
#include <string_view>
import Chess.Chessboard;
import Chess.ChessboardBuilder;
import Chess.Client.MoveClient;
import Chess.ePieceColor;
import Chess.GameStateChecker;
import Chess.Move;
import Chess.MoveValidator;
import Chess.PieceDirector;
import Chess.Player;
import Console.Chess.ChessboardPresenter;
import Console.Chess.ConsolePromoter;
import Console.Chess.Controller;
import Console.Chess.InputHandler;
import Console.Chess.Game;
import Console.Chess.LabelPresenter;

namespace
{
    int RunClientDemo()
    {
        try
        {
            const Chess::Move move{ .from = { .file = 'E', .rank = 2 }, .to = { .file = 'E', .rank = 4 } };
            const Chess::Move echoed = Chess::Client::MoveClient::SendMove("127.0.0.1", 5555, move);
            std::println("Server echoed: {}{} -> {}{}",
                         static_cast<char>(echoed.from.file),
                         echoed.from.rank,
                         static_cast<char>(echoed.to.file),
                         echoed.to.rank);
        }
        catch (const std::exception& exception)
        {
            std::println("Client error: {}", exception.what());
            return 1;
        }
        return 0;
    }
} // namespace

int main(int argc, char** argv)
{
    if (argc > 1 && std::string_view(argv[1]) == "client")
    {
        return RunClientDemo();
    }

    try
    {
        const auto path = std::filesystem::current_path().parent_path().parent_path().parent_path() / "resources" / "chessboard.json";

        auto       piecesOnBoard = Chess::ChessboardBuilder::InitBoard(path.string());
        auto const player        = std::make_shared<Chess::Player>(Chess::ePieceColor::WHITE);
        auto       director      = std::make_unique<Chess::PieceDirector>(piecesOnBoard, player);
        auto       validator     = std::make_unique<Chess::MoveValidator>(piecesOnBoard, player);
        const auto chessboard    = std::make_shared<Chess::Chessboard>(player, std::move(piecesOnBoard), std::move(director), std::move(validator));

        auto       controller          = std::make_unique<Console::Chess::Controller>(chessboard);
        const auto chessboardPresenter = std::make_shared<Console::Chess::ChessboardPresenter>(chessboard);
        const auto inputHandler        = std::make_shared<Console::Chess::InputHandler>();
        auto       labelPresenter      = std::make_unique<Console::Chess::LabelPresenter>(inputHandler);
        auto       promoter            = std::make_unique<Console::Chess::ConsolePromoter>();
        auto       gameStateChecker    = std::make_unique<Chess::GameStateChecker>();

        auto chess = Console::Chess::Game(
            chessboard,
            std::move(controller),
            chessboardPresenter,
            inputHandler,
            std::move(labelPresenter),
            std::move(promoter),
            std::move(gameStateChecker));

        chess.Init();
        chess.Play();
    }
    catch (const std::exception& e)
    {
        std::println("{}", e.what());
    }

    return 0;
}
