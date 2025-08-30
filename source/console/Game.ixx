module;
#include <filesystem>
#include <functional>
#include <iostream>
#include <memory>
#include <print>
export module Chess.Game;
import Chess.Chessboard;
import Chess.ChessboardPresenter;
import Chess.ConsolePromoter;
import Chess.Controller;
import Chess.Coordinate;
import Chess.DrawChecker;
import Chess.InputHandler;
import Chess.LabelDisplayer;
import Chess.Piece;
import Chess.PieceDirector;
import Chess.ChessboardBuilder;

namespace Chess
{
    export class Game
    {
        std::unique_ptr<ChessboardPresenter> m_chessboardDisplayer;
        std::shared_ptr<Chessboard>          m_chessboard;
        std::shared_ptr<Controller>          m_controller;
        std::shared_ptr<InputHandler>        m_inputHandler;
        std::unique_ptr<LabelPresenter>      m_labelDisplayer;
        std::shared_ptr<ConsolePromoter>     m_promoter = std::make_shared<ConsolePromoter>();

        static void HandleInput(const std::function<Coordinate()>& inputFunction, const std::function<bool(const Coordinate&)>& initFunction)
        {
            while (true)
            {
                if (initFunction(inputFunction()))
                {
                    break;
                }
            }
        }

        bool ContinueGame() const
        {
            const auto drawChecker = std::make_unique<DrawChecker>();

            if (!m_chessboard->GetMoveValidator()->GetPiecesCanMoveCount() && m_chessboard->GetPieceDirector()->GetIsCheck())
            {
                std::println("Checkmate!");
                return false;
            }
            if (drawChecker->IsDraw(m_chessboard))
            {
                std::println("Draw!");
                return false;
            }
            if (m_chessboard->GetPieceDirector()->GetIsCheck())
            {
                std::println("Check!");
            }

            return true;
        }

    public:
        Game(const std::filesystem::path& path)
            : m_inputHandler(std::make_shared<InputHandler>())
            , m_labelDisplayer(std::make_unique<LabelPresenter>(m_inputHandler))

        {
            std::vector<std::shared_ptr<Piece>> pieces;
            try
            {
                pieces = ChessboardBuilder::InitBoard(path.string());
            }
            catch (const std::exception& e)
            {
                std::cerr << e.what();
                return;
            }
            m_chessboard          = std::make_shared<Chessboard>(pieces);
            m_controller          = std::make_shared<Controller>(m_chessboard);
            m_chessboardDisplayer = std::make_unique<ChessboardPresenter>(m_chessboard);
            m_chessboardDisplayer->Show();
        }

        void Play() const
        {
            m_labelDisplayer->Init();
            const auto tryMovePiece = [this](const Coordinate& to) -> bool { return m_controller->TryMovePiece(to, m_promoter); };

            while (true)
            {
                if (!ContinueGame())
                {
                    break;
                }

                HandleInput(
                    std::bind(&InputHandler::EnterFrom, m_inputHandler), std::bind(&Controller::TryInitPiece, m_controller.get(), std::placeholders::_1));

                try
                {
                    HandleInput(std::bind(&InputHandler::EnterTo, m_inputHandler), tryMovePiece);
                }
                catch (const std::invalid_argument& e)
                {
                    std::cerr << e.what();
                }
            }
        }
    };
} // namespace Chess
