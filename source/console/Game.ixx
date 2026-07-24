module;
#include <memory>
#include <print>
export module Console.Chess.Game;
import Chess.Chessboard;
import Chess.Coordinate;
import Chess.Client.Session;
import Chess.eGameState;
import Chess.ePieceColor;
import Chess.ePieceType;
import Chess.Move;
import Console.Chess.ChessboardPresenter;
import Console.Chess.ConsolePromoter;
import Console.Chess.InputHandler;
import Console.Chess.LabelPresenter;

namespace Console::Chess
{
    export class Game
    {
    public:
        static void RunNetworkGame(::Chess::Client::Session& session)
        {
            const auto inputHandler   = std::make_shared<InputHandler>();
            auto       labelPresenter = LabelPresenter(inputHandler);
            labelPresenter.Init();
            const auto promoter = std::make_shared<ConsolePromoter>();

            std::println("You play {}.", session.GetMyColor() == ::Chess::ePieceColor::WHITE ? "White" : "Black");

            std::shared_ptr<::Chess::Chessboard> bound;
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

                auto promotion = ::Chess::ePieceType::NONE;
                if (IsPromotion(session.GetChessboard(), from, to))
                {
                    promotion = promoter->GetPromoteType();
                }

                if (!session.TrySubmitMove(::Chess::Move{ .from = from, .to = to, .promotion = promotion }))
                {
                    std::println("Illegal move.");
                    continue;
                }

                session.ReceiveNext();
            }
        }

    private:
        static bool IsPromotion(const std::shared_ptr<::Chess::Chessboard>& board, const ::Chess::Coordinate& from, const ::Chess::Coordinate& to)
        {
            const auto piece = board->GetPieceDirector()->GetPiece(from);
            if (!piece)
            {
                return false;
            }
            const auto [color, type] = piece->GetColorAndType();
            if (type != ::Chess::ePieceType::PAWN)
            {
                return false;
            }
            return (color == ::Chess::ePieceColor::WHITE && to.rank == 8) || (color == ::Chess::ePieceColor::BLACK && to.rank == 1);
        }

        static void PrintOutcome(::Chess::eGameState state)
        {
            switch (state)
            {
            case ::Chess::eGameState::CHECKMATE:
                std::println("Checkmate!");
                break;
            case ::Chess::eGameState::DRAW:
                std::println("Draw!");
                break;
            default:
                std::println("Game over.");
                break;
            }
        }
    };
} // namespace Console::Chess
