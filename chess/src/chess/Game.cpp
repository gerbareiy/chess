#include "Game.h"

#include "Chessboard.h"
#include "Sizes.h"

#include <stdlib.h>

Chess::Game::Game()
{
	auto chessboard = std::make_shared<Chessboard>();
	m_controller = std::make_unique<Controller>(chessboard);
	m_window = std::make_unique<Console>(chessboard);
}

void Chess::Game::Play()
{
	while (true)
	{
		system("CLS");

		m_window->DisplayChessboard();
		m_window->DisplayEmpty();

		auto from = m_window->EnterFrom();

		if (from.get_File() < 'A'
			|| from.get_File() > 'A' + CHESSBOARD_SIZE - 1
			|| from.get_Rank() < 1
			|| from.get_Rank() > CHESSBOARD_SIZE
			|| !m_controller->TryInitPiece(from))
		{
			m_window->ShowInvalidMovePrompt(false);
			continue;
		}

		system("CLS");

		m_window->DisplayChessboard();
		m_window->DisplayEmpty();

		auto to = m_window->EnterTo();

		if (to.get_File() < 'A' || to.get_File() > 'A' + CHESSBOARD_SIZE - 1 || to.get_Rank() < 1 || to.get_Rank() > CHESSBOARD_SIZE)
		{
			m_window->ShowInvalidMovePrompt(false);
			continue;
		}

		m_window->ShowInvalidMovePrompt(m_controller->TryMovePiece(to));
	}
}
