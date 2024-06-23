#pragma once

#include "ChessboardDisplayer.h"
#include "Controller.h"
#include "InputDisplayer.h"
#include "InputHandler.h"

#include <memory>
#include <variant>

namespace Chess
{
	class Game
	{
	private:
		std::unique_ptr<ChessboardDisplayer> m_chessboardDisplayer;
		std::shared_ptr<Controller> m_controller;
		std::shared_ptr<InputHandler> m_inputHandler;
		std::unique_ptr<InputDisplayer> m_inputDisplayer;

	public:
		Game();

    private:
		Coordinate HandleInput(std::function<Coordinate()> inputFunction, std::function<bool(const Coordinate&)> initFunction);

	public:
		void Play();
	};
}
