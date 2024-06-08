#pragma once

#include "Console.h"
#include "Controller.h"

#include <memory>

namespace Chess
{
	class Game
	{
	private:
		std::unique_ptr<Controller> m_controller;
		std::unique_ptr<Console> m_window;

	public:
		Game();
		~Game() {};

	public:
		void Play();
	};
}
