#pragma once

#include "pieces/logic/ePieceColor.h"

#include <memory>

namespace Chess
{
	class PieceSignalDirector;

	class Player
	{
	private:
		ePieceColor m_playerColor;

	public:
		Player(ePieceColor firstMoveColor, const std::shared_ptr<PieceSignalDirector>& signalDirector);

	private:
		void ChangeColor();

	public:
		ePieceColor GetPlayerColor();
	};
}
