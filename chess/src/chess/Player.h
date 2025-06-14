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
		Player(ePieceColor firstMoveColor, std::shared_ptr<PieceSignalDirector> const& signalDirector);

	private:
		void ChangeColor();

	public:
		ePieceColor GetPlayerColor();
	};
}
