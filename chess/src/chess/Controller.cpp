#include "Controller.h"

#include <iostream>

Chess::Controller::Controller(std::shared_ptr<Chessboard> chessboard)
	: m_chessboard(chessboard) { }

bool Chess::Controller::TryInitPiece(Coordinate from)
{
	return m_chessboard->TryInitPiece(from);
}

bool Chess::Controller::TryMovePiece(Coordinate to)
{
	return m_chessboard->TryMovePiece(to);
}
