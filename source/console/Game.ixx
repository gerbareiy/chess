module;
#include <memory>
#include <print>
export module Chess.Console.Game;
import Chess.Client.Session;
import Chess.Console.ChessboardPresenter;
import Chess.Console.ConsolePromoter;
import Chess.Console.InputHandler;
import Chess.Console.LabelPresenter;
import Chess.Core.Chessboard;
import Chess.Core.Coordinate;
import Chess.Core.eGameState;
import Chess.Core.ePieceColor;
import Chess.Core.ePieceType;
import Chess.Core.Move;

namespace Chess::Console
{
    export class Game
    {
    public:
        static void RunNetworkGame(Client::Session& session)
        {
            const auto inputHandler   = std::make_shared<InputHandler>();
            auto       labelPresenter = LabelPresenter(inputHandler);
            labelPresenter.Init();
            const auto promoter = std::make_shared<ConsolePromoter>();

            std::println("You play {}.", session.GetMyColor() == Core::ePieceColor::WHITE ? "White" : "Black");

            std::shared_ptr<Core::Chessboard>    bound;
            std::shared_ptr<ChessboardPresenter> presenter;
            const auto                           render = [&]
            {
                if (bound != session.GetChessboard())
                {
                    bound     = session.GetChessboard();
                    presenter = std::make_shared<ChessboardPresenter>(bound);
                    presenter->Init();
                }
                presenter->Show();
            };

            while (true)
            {
                render();

                if (session.IsGameOver())
                {
                    if (session.GetOpponentLeft())
                    {
                        std::println("Opponent left the game.");
                    }
                    else
                    {
                        PrintOutcome(session.GetFinalState());
                    }
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

                auto promotion = Core::ePieceType::NONE;
                if (IsPromotion(session.GetChessboard(), from, to))
                {
                    promotion = promoter->GetPromoteType();
                }

                if (!session.TrySubmitMove(Core::Move{ .from = from, .to = to, .promotion = promotion }))
                {
                    std::println("Illegal move.");
                    continue;
                }

                session.ReceiveNext();
            }
        }

    private:
        static bool IsPromotion(const std::shared_ptr<Core::Chessboard>& board, const Core::Coordinate& from, const Core::Coordinate& to)
        {
            const auto piece = board->GetPieceDirector()->GetPiece(from);
            if (!piece)
            {
                return false;
            }
            const auto [color, type] = piece->GetColorAndType();
            if (type != Core::ePieceType::PAWN)
            {
                return false;
            }
            return (color == Core::ePieceColor::WHITE && to.rank == 8) || (color == Core::ePieceColor::BLACK && to.rank == 1);
        }

        static void PrintOutcome(Core::eGameState state)
        {
            switch (state)
            {
            case Core::eGameState::CHECKMATE:
                std::println("Checkmate!");
                break;
            case Core::eGameState::DRAW:
                std::println("Draw!");
                break;
            default:
                std::println("Game over.");
                break;
            }
        }
    };
} // namespace Chess::Console
