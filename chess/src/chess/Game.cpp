#include "Game.h"

#include "pieces/logic/PositionChecker.h"
#include "logic/DrawChecker.h"
#include "logic/Sizes.h"
#include "pieces/logic/ePieceColor.h"

#include <stdlib.h>

Chess::Game::Game()
{
	m_chessboard = std::make_shared<Chessboard>();
	m_controller = std::make_shared<Controller>(m_chessboard);
	m_chessboardDisplayer = std::make_unique<ChessboardDisplayer>(m_chessboard);
	m_handlerInputer = std::make_shared<HandlerInputer>();
	m_inputDisplayer = std::make_unique<LableDisplayer>(m_handlerInputer);

	m_chessboardDisplayer->Show();
}

bool Chess::Game::IsGameContinue() const
{
	auto drawChecker = std::make_unique<DrawChecker>();

	if (!m_chessboard->get_MoveValidator()->GetPiecesCanMoveCount() && m_chessboard->get_PieceDirector()->get_IsCheck())
	{
		m_inputDisplayer->Show("Checkmate!\n");
		return false;
	}
	if (drawChecker->IsDraw(m_chessboard))
	{
		m_inputDisplayer->Show("Draw!\n");
		return false;
	}
	if (m_chessboard->get_PieceDirector()->get_IsCheck())
	{
		m_inputDisplayer->Show("Check!\n");
	}

	return true;
}

void Chess::Game::HandleInput(std::function<Coordinate()> inputFunction, std::function<bool(const Coordinate&)> initFunction) const
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
		if (!IsGameContinue())
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
