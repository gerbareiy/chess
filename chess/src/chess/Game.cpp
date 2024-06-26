#include "Game.h"

#include "Chessboard.h"
#include "pieces/logic/PositionChecker.h"
#include "logic/Sizes.h"
#include "pieces/logic/ePieceColor.h"

#include <stdlib.h>

Chess::Game::Game()
{
	auto chessboard = std::make_shared<Chessboard>();
	m_controller = std::make_shared<Controller>(chessboard);
	m_chessboardDisplayer = std::make_unique<ChessboardDisplayer>(chessboard);
	m_handlerInputer = std::make_shared<HandlerInputer>();
	m_inputDisplayer = std::make_unique<InputerDisplayer>(m_handlerInputer);

	m_chessboardDisplayer->Show();
}

Chess::Coordinate Chess::Game::HandleInput(std::function<Coordinate()> inputFunction, std::function<bool(const Coordinate&)> initFunction) const
{
	Coordinate coordinate;

	while (true)
	{
		coordinate = inputFunction();

		if (PositionChecker::IsPositionValid(coordinate) && initFunction(coordinate))
		{
			break;
		}
	}
	return coordinate;
}

void Chess::Game::Play() const
{
	while (true)
	{
		Coordinate from = HandleInput(
			std::bind(&HandlerInputer::EnterFrom, m_handlerInputer),
			std::bind(&Controller::TryInitPiece, m_controller.get(), std::placeholders::_1)

		);

		Coordinate to = HandleInput(
			std::bind(&HandlerInputer::EnterTo, m_handlerInputer),
			std::bind(&Controller::TryMovePiece, m_controller.get(), std::placeholders::_1)
		);
	}
}
