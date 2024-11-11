#include "Player.h"

#include "logic/PieceSignalDirector.h"

#include <iostream>

Chess::ePieceColor Chess::Player::get_PlayerColor()
{
	return m_playerColor;
}

Chess::Player::Player(ePieceColor firstMoveColor, const std::shared_ptr<PieceSignalDirector>& signalDirector)
	: m_playerColor(firstMoveColor == ePieceColor::NONE ? ePieceColor::WHITE : firstMoveColor)
{
	signalDirector->ConnectMove(std::bind(&Player::ChangeColor, this));
}

void Chess::Player::ChangeColor()
{
	m_playerColor = m_playerColor == ePieceColor::BLACK ? ePieceColor::WHITE : ePieceColor::BLACK;
}
