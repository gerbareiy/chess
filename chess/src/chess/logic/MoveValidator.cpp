#include "MoveValidator.h"

Chess::MoveValidator::MoveValidator()
{
	m_checker = std::make_shared<MoveChecker>();
}

std::vector<Chess::Coordinate> Chess::MoveValidator::get_PossibleMoves()
{
	return m_possibleMoves;
}

void Chess::MoveValidator::CalculatePossibleMoves(std::shared_ptr<IPiece> piece, std::vector<std::shared_ptr<IPiece>> piecesOnBoard)
{
	m_possibleMoves = m_checker->GetPossibleMoves(piece, piecesOnBoard);
}

void Chess::MoveValidator::ClearPossibleMoves()
{
	m_possibleMoves.clear();
}

bool Chess::MoveValidator::IsCoordinateInPossibleMoves(Coordinate coordinate)
{
	auto it = std::find(m_possibleMoves.begin(), m_possibleMoves.end(), coordinate);

	return it != m_possibleMoves.end();
}

bool Chess::MoveValidator::IsValidMove(std::shared_ptr<IPiece> piece, Coordinate to)
{
	if (!piece)
	{
		return false;
	}

	auto it = std::find(m_possibleMoves.begin(), m_possibleMoves.end(), to);

	if (it == m_possibleMoves.end())
	{
		return false;
	}

	return true;
}