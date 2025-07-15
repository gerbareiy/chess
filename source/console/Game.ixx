module;
#include <functional>
#include <memory>
export module Chess.Game;
import Chess.Chessboard;
import Chess.ChessboardDisplayer;
import Chess.ConsolePromoter;
import Chess.Controller;
import Chess.Coordinate;
import Chess.DrawChecker;
import Chess.InputHandler;
import Chess.LabelDisplayer;
import Chess.PieceDirector;
import Chess.PieceInitializer;
import Chess.PieceSignalDirector;

namespace Chess
{
    export class Game
    {
        std::unique_ptr<ChessboardDisplayer> m_chessboardDisplayer;
        std::shared_ptr<Chessboard>          m_chessboard;
        std::shared_ptr<Controller>          m_controller;
        std::shared_ptr<InputHandler>        m_inputHandler;
        std::unique_ptr<LabelDisplayer>      m_labelDisplayer;
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
                LabelDisplayer::Show("Checkmate!\n");
                return false;
            }
            if (drawChecker->IsDraw(m_chessboard))
            {
                LabelDisplayer::Show("Draw!\n");
                return false;
            }
            if (m_chessboard->GetPieceDirector()->GetIsCheck())
            {
                LabelDisplayer::Show("Check!\n");
            }

            return true;
        }

    public:
        Game()
            : m_inputHandler(std::make_shared<InputHandler>())
            , m_labelDisplayer(std::make_unique<LabelDisplayer>(m_inputHandler))

        {
            auto signalDirector   = std::make_shared<PieceSignalDirector>();
            m_chessboard          = std::make_shared<Chessboard>(std::make_shared<PieceInitializer>()->InitNormalBoard(signalDirector), signalDirector);
            m_controller          = std::make_shared<Controller>(m_chessboard);
            m_chessboardDisplayer = std::make_unique<ChessboardDisplayer>(m_chessboard);

            m_chessboardDisplayer->Show();
        }

        void Play() const
        {
            m_labelDisplayer->Init();
            const auto tryMovePiece = [this](const Coordinate& to) -> bool
            {
                return m_controller->TryMovePiece(to, m_promoter);
            };

            while (true)
            {
                if (!ContinueGame())
                {
                    break;
                }

                HandleInput(std::bind(&InputHandler::EnterFrom, m_inputHandler),
                            std::bind(&Controller::TryInitPiece, m_controller.get(), std::placeholders::_1));

                HandleInput(std::bind(&InputHandler::EnterTo, m_inputHandler), tryMovePiece);
            }
        }
    };
} // namespace Chess
