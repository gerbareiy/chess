#include <filesystem>
#include <print>
import Chess.Chessboard;
import Chess.ChessboardBuilder;
import Chess.ChessboardPresenter;
import Chess.ChessboardPresenter;
import Chess.ConsolePromoter;
import Chess.Controller;
import Chess.ePieceColor;
import Chess.Game;
import Chess.InputHandler;
import Chess.LabelPresenter;
import Chess.MoveValidator;
import Chess.PieceDirector;
import Chess.Player;

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
        chessboard->Init();

        auto       controller          = std::make_unique<Chess::Controller>(chessboard);
        auto       chessboardPresenter = std::make_unique<Chess::ChessboardPresenter>(chessboard);
        const auto inputHandler        = std::make_shared<Chess::InputHandler>();
        auto       labelPresenter      = std::make_unique<Chess::LabelPresenter>(inputHandler);
        auto       promoter            = std::make_unique<Chess::ConsolePromoter>();
        auto       chess =
            Chess::Game(chessboard, std::move(controller), std::move(chessboardPresenter), inputHandler, std::move(labelPresenter), std::move(promoter));
        chess.Init();
        chess.Play();
    }
    catch (const std::exception& e)
    {
        std::println("{}", e.what());
    }

    return 0;
}
