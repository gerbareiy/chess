module;
#include <filesystem>
#include <functional>
#include <memory>
#include <print>
export module Console.Chess.Game;
import Chess.Chessboard;
import Chess.ChessboardBuilder;
import Chess.Coordinate;
import Chess.DrawChecker;
import Chess.Piece;
import Chess.PieceDirector;
import Console.Chess.ChessboardPresenter;
import Console.Chess.ConsolePromoter;
import Console.Chess.Controller;
import Console.Chess.InputHandler;
import Console.Chess.LabelPresenter;

namespace Console::Chess
{
    export class Game
    {
        std::shared_ptr<::Chess::Chessboard>  m_chessboard;
        std::shared_ptr<Controller>           m_controller;
        std::shared_ptr<ChessboardPresenter>  m_chessboardPresenter;
        std::shared_ptr<InputHandler>         m_inputHandler;
        std::unique_ptr<LabelPresenter>       m_labelPresenter;
        std::shared_ptr<ConsolePromoter>      m_promoter;
        std::unique_ptr<::Chess::DrawChecker> m_drawChecker;

        static void HandleInput(
            const std::function<::Chess::Coordinate()>& inputFunction, const std::function<bool(const ::Chess::Coordinate&)>& initFunction)
        {
            while (true)
            {
                if (initFunction(inputFunction()))
                {
                    break;
                }
            }
        }

        bool TryContinue() const
        {
            if (!m_chessboard->GetMoveValidator()->GetPiecesCanMoveCount() && m_chessboard->GetPieceDirector()->GetIsCheck())
            {
                std::println("Checkmate!");
                return false;
            }
            if (m_drawChecker->IsDraw(m_chessboard))
            {
                std::println("Draw!");
                return false;
            }
            if (m_chessboard->GetPieceDirector()->GetIsCheck())
            {
                std::println("Check!");
            }
            if (false)
            {
                std::println("Game broken!");
            }
            return true;
        }

    public:
        Game(
            const std::shared_ptr<::Chess::Chessboard>& chessboard,
            std::unique_ptr<Controller>&&               controller,
            const std::shared_ptr<ChessboardPresenter>& chessboardPresenter,
            const std::shared_ptr<InputHandler>&        inputHandler,
            std::unique_ptr<LabelPresenter>&&           labelPresenter,
            std::unique_ptr<ConsolePromoter>&&          promoter,
            std::unique_ptr<::Chess::DrawChecker>&&     drawChecker)
            : m_chessboard(chessboard)
            , m_controller(std::move(controller))
            , m_chessboardPresenter(chessboardPresenter)
            , m_inputHandler(inputHandler)
            , m_labelPresenter(std::move(labelPresenter))
            , m_promoter(std::move(promoter))
            , m_drawChecker(std::move(drawChecker))
        {
        }

        void Init()
        {
            m_chessboard->Init();
            m_chessboardPresenter->Init();
            m_chessboardPresenter->Show();
            m_labelPresenter->Init();
        }

        void Play()
        {
            const auto trySelectPiece = std::bind(&Controller::TrySelectPiece, m_controller.get(), std::placeholders::_1);
            const auto tryMovePiece   = [this](const ::Chess::Coordinate& to) -> bool
            {
                return m_controller->TryMovePiece(to, m_promoter);
            };
            while (TryContinue())
            {
                HandleInput(std::bind(&InputHandler::EnterFrom, m_inputHandler), trySelectPiece);
                HandleInput(std::bind(&InputHandler::EnterTo, m_inputHandler), tryMovePiece);
            }
        }
    };
} // namespace Console::Chess
