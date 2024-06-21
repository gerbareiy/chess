#include "MoveChecker.h"

#include "MoveCheckerFactory.h"

#include "stdexcept"
#include <vector>

Chess::MoveChecker::MoveChecker(const std::shared_ptr<IPiece>& piece)
    : m_piece(piece), m_moveChecker(std::unique_ptr<MoveCheckerFactory>()->Create(piece)) { }

std::vector<Chess::Coordinate> Chess::MoveChecker::GetPossibleMoves(const std::vector<std::shared_ptr<IPiece>>& piecesOnBoard)
{
    return m_moveChecker->GetPossibleMoves(m_piece, piecesOnBoard);
}
