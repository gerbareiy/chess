#include "Game.h"

#include "Chessboard.h"
#include "Controller.h"
#include "logic/DrawChecker.h"
#include "logic/MoveValidator.h"
#include "logic/PieceDirector.h"
#include "logic/PieceInitializer.h"
#include "logic/PieceSignalDirector.h"

Chess::Game::Game()
{
	auto signalDirector = std::make_shared<PieceSignalDirector>();
	m_chessboard = std::make_shared<Chessboard>(std::make_shared<PieceInitializer>()->InitNormalBoard(signalDirector), signalDirector);
	m_controller = std::make_shared<Controller>(m_chessboard);
	m_chessboardDisplayer = std::make_unique<ChessboardDisplayer>(m_chessboard);
	m_handlerInputer = std::make_shared<HandlerInputer>();
	m_inputDisplayer = std::make_unique<LableDisplayer>(m_handlerInputer);

	m_chessboardDisplayer->Show();
}

bool Chess::Game::ContinueGame() const
{
	auto drawChecker = std::make_unique<DrawChecker>();

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

void Chess::Game::HandleInput(std::function<Coordinate()> const& inputFunction, std::function<bool(Coordinate const&)> const& initFunction) const
{
	while (true)
	{
		if (initFunction(inputFunction()))
		{
			break;
		}
	}
}

void Chess::Game::Play() const
{
	while (true)
	{
		if (!ContinueGame())
		{
			break;
		}
		
		HandleInput(
			std::bind(&HandlerInputer::EnterFrom, m_handlerInputer),
			std::bind(&Controller::TryInitPiece, m_controller.get(), std::placeholders::_1));

		HandleInput(
			std::bind(&HandlerInputer::EnterTo, m_handlerInputer),
			std::bind(&Controller::TryMovePiece, m_controller.get(), std::placeholders::_1));
	}
}
