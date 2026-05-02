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
import Chess.LabelPresenter;
import Chess.Piece;
import Chess.PieceDirector;
import Chess.ChessboardBuilder;

namespace Chess
{
    export class Game
    {
        std::shared_ptr<Chessboard>          m_chessboard;
        std::shared_ptr<Controller>          m_controller;
        std::shared_ptr<ChessboardPresenter> m_chessboardPresenter;
        std::shared_ptr<InputHandler>        m_inputHandler;
        std::unique_ptr<LabelPresenter>      m_labelPresenter;
        std::shared_ptr<ConsolePromoter>     m_promoter;

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
        Game(
            const std::shared_ptr<Chessboard>&     chessboard,
            std::unique_ptr<Controller>&&          controller,
            std::unique_ptr<ChessboardPresenter>&& chessboardPresenter,
            const std::shared_ptr<InputHandler>&   inputHandler,
            std::unique_ptr<LabelPresenter>&&      labelPresenter,
            std::unique_ptr<ConsolePromoter>&&     promoter)
            : m_chessboard(chessboard)
            , m_controller(std::move(controller))
            , m_chessboardPresenter(std::move(chessboardPresenter))
            , m_inputHandler(inputHandler)
            , m_labelPresenter(std::move(labelPresenter))
            , m_promoter(std::move(promoter))
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
