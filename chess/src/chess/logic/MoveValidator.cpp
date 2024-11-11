#include "MoveValidator.h"

#include "Coordinate.h"
#include "Counts.h"
#include "../pieces/Piece.h"
#include "../pieces/logic/CheckChecker.h"
#include "../pieces/logic/MoveChecker.h"
#include "../pieces/logic/PieceColorAndType.h"
#include "../pieces/logic/PieceFinder.h"
#include "../Player.h"

Chess::MoveValidator::MoveValidator(const std::vector<std::shared_ptr<Piece>>& piecesOnBoard, const std::shared_ptr<Player>& player)
	: m_piecesOnBoard(piecesOnBoard), m_player(player)
{
	CalculatePiecesCanMove();
}

std::vector<Chess::Coordinate> Chess::MoveValidator::get_PossibleMoves()
{
	return m_possibleMoves;
}

void Chess::MoveValidator::CalculatePiecesCanMove()
{
	ClearPiecesCanMove();
	ClearPossibleMoves();

	std::vector<std::shared_ptr<Chess::Piece>> pieces;
	pieces.reserve(MAX_COUNT_ELEMENTS);

	for (const auto& piece : m_piecesOnBoard)
	{
		if (piece->get_ColorAndType().get_Color() == m_player->get_PlayerColor())
		{
			const auto moveChecker = std::make_unique<MoveChecker>(piece);

			if (moveChecker->GetFilteredMoves(m_piecesOnBoard).size())
			{
				pieces.emplace_back(piece);
			}
		}
	}

	m_piecesCanMove = pieces;
}

void Chess::MoveValidator::CalculatePossibleMoves(const std::shared_ptr<Piece>& piece)
{
	auto it = std::find(m_piecesCanMove.begin(), m_piecesCanMove.end(), piece);

	if (it != m_piecesCanMove.end())
	{
		auto moveChecker = std::make_shared<MoveChecker>(piece);
		m_possibleMoves = moveChecker->GetFilteredMoves(m_piecesOnBoard);
	}
}

void Chess::MoveValidator::ClearPossibleMoves()
{
	m_possibleMoves.clear();
}

void Chess::MoveValidator::ClearPiecesCanMove()
{
	m_piecesCanMove.clear();
}

int Chess::MoveValidator::GetPiecesCanMoveCount()
{
	return m_piecesCanMove.size();
}

bool Chess::MoveValidator::IsCoordinateInPieceCanMove(Coordinate coordinate) const
{
	auto finder = std::make_unique<PieceFinder>(m_piecesCanMove);

	return !!finder->Find(coordinate);
}

bool Chess::MoveValidator::IsCoordinateInPossibleMoves(Coordinate coordinate) const
{
	auto it = std::find(m_possibleMoves.begin(), m_possibleMoves.end(), coordinate);

	return it != m_possibleMoves.end();
}
 
bool Chess::MoveValidator::IsValidMove(const std::shared_ptr<Piece> piece, Coordinate to) const
{
	if (!piece)
	{
		return false;
	}

	return std::find(m_possibleMoves.begin(), m_possibleMoves.end(), to) != m_possibleMoves.end();
}
