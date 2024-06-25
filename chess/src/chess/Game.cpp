#include "Game.h"

#include "Chessboard.h"
#include "pieces/logic/PositionChecker.h"
#include "logic/Sizes.h"
#include "pieces/logic/ePieceColor.h"

#include <stdlib.h>

Chess::Game::Game()
{
	auto chessboard = std::make_shared<Chessboard>();
	m_chessboardDisplayer = std::make_unique<ChessboardDisplayer>(chessboard);
	m_controller = std::make_shared<Controller>(chessboard);
	m_inputHandler = std::make_shared<InputHandler>();
	m_inputDisplayer = std::make_unique<InputDisplayer>(m_inputHandler);
}

Chess::Coordinate Chess::Game::HandleInput(std::function<Coordinate()> inputFunction, std::function<bool(const Coordinate&)> initFunction) const
{
	Coordinate coordinate;

	while (true)
	{
		system("CLS");

		m_chessboardDisplayer->Show();

		coordinate = inputFunction();

		if (!(PositionChecker::IsPositionValid(coordinate) && initFunction(coordinate)))
		{
			m_chessboardDisplayer->ShowInvalidMovePrompt(false);
			continue;
		}

		break;
	}
	return coordinate;
}

void Chess::Game::Play() const
{
	while (true)
	{
		Coordinate from = HandleInput(
			std::bind(&InputHandler::EnterFrom, m_inputHandler),
			std::bind(&Controller::TryInitPiece, m_controller.get(), std::placeholders::_1)

		);

		Coordinate to = HandleInput(
			std::bind(&InputHandler::EnterTo, m_inputHandler),
			std::bind(&Controller::TryMovePiece, m_controller.get(), std::placeholders::_1)
		);
	}
}
