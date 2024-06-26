#pragma once

#include "ChessboardDisplayer.h"
#include "Controller.h"
#include "LableDisplayer.h"
#include "HandlerInputer.h"

#include <memory>
#include <variant>

namespace Chess
{
	class Game
	{
	private:
		std::unique_ptr<ChessboardDisplayer> m_chessboardDisplayer;
		std::shared_ptr<Controller> m_controller;
		std::shared_ptr<HandlerInputer> m_handlerInputer;
		std::unique_ptr<LableDisplayer> m_inputDisplayer;

	public:
		Game();

    private:
		void HandleInput(std::function<Coordinate()> inputFunction, std::function<bool(const Coordinate&)> initFunction) const;

	public:
		void Play() const;
	};
}
