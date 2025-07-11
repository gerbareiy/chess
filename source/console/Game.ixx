module;
#include "core/logic/Coordinate.h"

#include <functional>
#include <memory>
export module Chess.Game;
import Chess.Chessboard;
import Chess.ChessboardDisplayer;
import Chess.Controller;
import Chess.DrawChecker;
import Chess.HandlerInputer;
import Chess.LabelShower;
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
        std::shared_ptr<HandlerInputer>      m_handlerInputer;
        std::unique_ptr<LabelShower>         m_inputDisplayer;

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
                LabelShower::Show("Checkmate!\n");
                return false;
            }
            if (drawChecker->IsDraw(m_chessboard))
            {
                LabelShower::Show("Draw!\n");
                return false;
            }
            if (m_chessboard->GetPieceDirector()->GetIsCheck())
            {
                LabelShower::Show("Check!\n");
            }

            return true;
        }

    public:
        Game()
        {
            auto signalDirector   = std::make_shared<PieceSignalDirector>();
            m_chessboard          = std::make_shared<Chessboard>(std::make_shared<PieceInitializer>()->InitNormalBoard(signalDirector), signalDirector);
            m_controller          = std::make_shared<Controller>(m_chessboard);
            m_chessboardDisplayer = std::make_unique<ChessboardDisplayer>(m_chessboard);
            m_handlerInputer      = std::make_shared<HandlerInputer>();
            m_inputDisplayer      = std::make_unique<LabelShower>(m_handlerInputer);

            m_chessboardDisplayer->Show();
        }

        void Play() const
        {
            while (true)
            {
                if (!ContinueGame())
                {
                    break;
                }

                HandleInput(std::bind(&HandlerInputer::EnterFrom, m_handlerInputer),
                            std::bind(&Controller::TryInitPiece, m_controller.get(), std::placeholders::_1));

                HandleInput(std::bind(&HandlerInputer::EnterTo, m_handlerInputer),
                            std::bind(&Controller::TryMovePiece, m_controller.get(), std::placeholders::_1));
            }
        }
    };
} // namespace Chess
