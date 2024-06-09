#include "Controller.h"

#include <iostream>

Chess::Controller::Controller(std::shared_ptr<Chessboard> chessboard)
	: m_chessboard(chessboard) { }

bool Chess::Controller::TryMovePiece(Coordinate from, Coordinate to)
{
	return m_chessboard->TryMovePiece(from, to);
}
