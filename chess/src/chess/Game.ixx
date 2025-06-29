module;
#include "Chessboard.h"
#include "ChessboardDisplayer.h"
#include "Controller.h"
#include "HandlerInputer.h"
#include "LabelShower.h"
#include "logic/DrawChecker.h"
#include "logic/MoveValidator.h"
#include "logic/PieceDirector.h"
#include "logic/PieceInitializer.h"
#include "logic/PieceSignalDirector.h"

#include <memory>
export module Chess.Game;

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
                m_inputDisplayer->Show("Checkmate!\n");
                return false;
            }
            if (drawChecker->IsDraw(m_chessboard))
            {
                m_inputDisplayer->Show("Draw!\n");
                return false;
            }
            if (m_chessboard->GetPieceDirector()->GetIsCheck())
            {
                m_inputDisplayer->Show("Check!\n");
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
