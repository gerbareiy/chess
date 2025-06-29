#include "Player.h"

#include "logic/PieceSignalDirector.h"

void Chess::Player::ChangeColor()
{
    m_playerColor = m_playerColor == ePieceColor::BLACK ? ePieceColor::WHITE : ePieceColor::BLACK;
}

Chess::Player::Player(ePieceColor firstMoveColor, const std::shared_ptr<PieceSignalDirector>& signalDirector)
    : m_playerColor(firstMoveColor == ePieceColor::NONE ? ePieceColor::WHITE : firstMoveColor)
{
    signalDirector->ConnectMove(std::bind(&Player::ChangeColor, this));
}

Chess::ePieceColor Chess::Player::GetPlayerColor()
{
    return m_playerColor;
}
