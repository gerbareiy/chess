#pragma once

#include "InputHandler.h"
#include "Controller.h"
#include "ChessboardDisplayer.h"

#include <memory>

namespace Chess
{
	class Game
	{
	private:
		std::unique_ptr<Controller> m_controller;
		std::unique_ptr<ChessboardDisplayer> m_displayer;
		std::unique_ptr<InputHandler> m_inputHandler;

	public:
		Game();

	public:
		void Play();
	};
}
