#pragma once

#include "logic/PieceSignalDirector.h"
#include "pieces/logic/ePieceColor.h"

#include <memory>

namespace Chess
{
	class Player
	{
	private:
		ePieceColor m_playerColor;

	public:
		ePieceColor get_PlayerColor();

	public:
		Player(ePieceColor firstMoveColor, const std::shared_ptr<PieceSignalDirector>& signalDirector);

	private:
		void ChangeColor();
	};
}
