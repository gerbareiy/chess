#include "Player.h"

#include "logic/PieceSignalDirector.h"

#include <iostream>

Chess::Player::Player(ePieceColor firstMoveColor, std::shared_ptr<PieceSignalDirector> const& signalDirector)
	: m_playerColor(firstMoveColor == ePieceColor::NONE ? ePieceColor::WHITE : firstMoveColor)
{
	signalDirector->ConnectMove(std::bind(&Player::ChangeColor, this));
}

void Chess::Player::ChangeColor()
{
	m_playerColor = m_playerColor == ePieceColor::BLACK ? ePieceColor::WHITE : ePieceColor::BLACK;
}

Chess::ePieceColor Chess::Player::GetPlayerColor()
{
	return m_playerColor;
}
