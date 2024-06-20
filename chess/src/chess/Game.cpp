#include "Game.h"

#include "Chessboard.h"
#include "logic/Sizes.h"

#include <stdlib.h>

Chess::Game::Game()
{
	auto chessboard = std::make_shared<Chessboard>();
	m_controller = std::make_unique<Controller>(chessboard);
	m_displayer = std::make_unique<ChessboardDisplayer>(chessboard);
	m_inputHandler = std::make_unique<InputHandler>();
}

void Chess::Game::Play()
{
	while (true)
	{
		Coordinate from;
		while (true)
		{
			system("CLS");

			m_displayer->Show();

			from = m_inputHandler->EnterFrom();

			auto isCorrectInit = from.get_File() < 'A' || from.get_File() > 'A' + CHESSBOARD_SIZE - 1
				|| from.get_Rank() < 1 || from.get_Rank() > CHESSBOARD_SIZE
				|| !m_controller->TryInitPiece(from);

			if (isCorrectInit)
			{
				m_displayer->ShowInvalidMovePrompt(false);
				continue;
			}

			break;
		}
		
		Coordinate to;
		while (true)
		{
			system("CLS");

			m_displayer->Show();

			to = m_inputHandler->EnterTo();

			auto isCorrectMove = to.get_File() < 'A' || to.get_File() > 'A' + CHESSBOARD_SIZE - 1
				|| to.get_Rank() < 1 || to.get_Rank() > CHESSBOARD_SIZE
				|| !m_controller->TryMovePiece(to);

			if (isCorrectMove)
			{
				m_displayer->ShowInvalidMovePrompt(false);
				continue;
			}

			break;
		}
	}
}
