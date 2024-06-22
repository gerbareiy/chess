#include "Player.h"

#include <iostream>

Chess::ePieceColor Chess::Player::get_PlayerColor()
{
	return m_playerColor;
}

Chess::Player::Player(ePieceColor firstMoveColor, const std::shared_ptr<Controller>& controller)
	: m_playerColor(firstMoveColor == ePieceColor::NONE ? ePieceColor::WHITE : firstMoveColor)
{
	controller->ConnectMove(std::bind(&Player::ChangeColor, this));
}

void Chess::Player::ChangeColor()
{
	m_playerColor = m_playerColor == ePieceColor::BLACK ? ePieceColor::WHITE : ePieceColor::BLACK;
}
