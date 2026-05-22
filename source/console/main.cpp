#include <filesystem>
#include <print>
import Chess.Chessboard;
import Chess.ChessboardBuilder;
import Chess.DrawChecker;
import Chess.ePieceColor;
import Chess.MoveValidator;
import Chess.PieceDirector;
import Chess.Player;
import Console.Chess.ChessboardPresenter;
import Console.Chess.ConsolePromoter;
import Console.Chess.Controller;
import Console.Chess.InputHandler;
import Console.Chess.Game;
import Console.Chess.LabelPresenter;

int main()
{
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
        auto       drawChecker         = std::make_unique<Chess::DrawChecker>();

        auto chess = Console::Chess::Game(
            chessboard, std::move(controller), chessboardPresenter, inputHandler, std::move(labelPresenter), std::move(promoter), std::move(drawChecker));
        
        chess.Init();
        chess.Play();
    }
    catch (const std::exception& e)
    {
        std::println("{}", e.what());
    }

    return 0;
}
