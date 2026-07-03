#include <exception>
#include <memory>
#include <print>
import Chess.Chessboard;
import Chess.Client.Session;
import Chess.Coordinate;
import Chess.eGameState;
import Chess.ePieceColor;
import Chess.ePieceType;
import Chess.Move;
import Chess.MoveValidator;
import Chess.Piece;
import Chess.PieceColorAndType;
import Chess.PieceDirector;
import Console.Chess.ChessboardPresenter;
import Console.Chess.ConsolePromoter;
import Console.Chess.InputHandler;
import Console.Chess.LabelPresenter;

namespace
{
    bool IsPromotion(const std::shared_ptr<Chess::Chessboard>& board, const Chess::Coordinate& from, const Chess::Coordinate& to)
    {
        const auto piece = board->GetPieceDirector()->GetPiece(from);
        if (!piece)
        {
            return false;
        }
        const auto [color, type] = piece->GetColorAndType();
        if (type != Chess::ePieceType::PAWN)
        {
            return false;
        }
        return (color == Chess::ePieceColor::WHITE && to.rank == 8) || (color == Chess::ePieceColor::BLACK && to.rank == 1);
    }

    void PrintOutcome(Chess::eGameState state)
    {
        switch (state)
        {
        case Chess::eGameState::CHECKMATE:
            std::println("Checkmate!");
            break;
        case Chess::eGameState::DRAW:
            std::println("Draw!");
            break;
        default:
            std::println("Game over.");
            break;
        }
    }

    void RunNetworkGame(Chess::Client::Session& session)
    {
        const auto inputHandler   = std::make_shared<Console::Chess::InputHandler>();
        auto       labelPresenter = std::make_unique<Console::Chess::LabelPresenter>(inputHandler);
        labelPresenter->Init();
        const auto promoter = std::make_shared<Console::Chess::ConsolePromoter>();

        std::println("You play {}.", session.GetMyColor() == Chess::ePieceColor::WHITE ? "White" : "Black");

        std::shared_ptr<Chess::Chessboard>                   bound;
        std::shared_ptr<Console::Chess::ChessboardPresenter> presenter;
        const auto                                           render = [&]()
        {
            if (bound != session.GetChessboard())
            {
                bound     = session.GetChessboard();
                presenter = std::make_shared<Console::Chess::ChessboardPresenter>(bound);
                presenter->Init();
            }
            presenter->Show();
        };

        while (true)
        {
            render();

            if (session.IsGameOver())
            {
                PrintOutcome(session.GetFinalState());
                break;
            }

            if (!session.IsMyTurn())
            {
                std::println("Waiting for opponent...");
                session.ReceiveNext();
                continue;
            }

            if (session.GetChessboard()->GetMoveValidator()->GetPiecesCanMoveCount() == 0)
            {
                std::println("No moves available. Waiting for result...");
                session.ReceiveNext();
                continue;
            }

            const auto from = inputHandler->EnterFrom();
            if (!session.GetChessboard()->TrySelectPiece(from))
            {
                std::println("No movable piece there.");
                continue;
            }
            render();

            const auto to = inputHandler->EnterTo();

            auto promotion = Chess::ePieceType::NONE;
            if (IsPromotion(session.GetChessboard(), from, to))
            {
                promotion = promoter->GetPromoteType();
            }

            if (!session.SubmitMove(Chess::Move{ .from = from, .to = to, .promotion = promotion }))
            {
                std::println("Illegal move.");
                continue;
            }

            session.ReceiveNext();
        }
    }
} // namespace

int main()
{
    try
    {
        const auto session = Chess::Client::Session::Connect("127.0.0.1", 5555);
        RunNetworkGame(*session);
    }
    catch (const std::exception& exception)
    {
        std::println("{}", exception.what());
    }

    return 0;
}
