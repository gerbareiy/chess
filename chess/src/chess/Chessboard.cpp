#include "Chessboard.h"

#include "logic/Counts.h"
#include "pieces/Bishop.h"
#include "pieces/logic/ePieceColor.h"
#include "pieces/IPiece.h"
#include "pieces/King.h"
#include "pieces/Knight.h"
#include "pieces/Pawn.h"
#include "pieces/Queen.h"
#include "pieces/Rook.h"
#include "logic/Sizes.h"

Chess::Chessboard::Chessboard()
{
	m_director = std::make_shared<PieceDirector>();
	m_validator = std::make_shared<MoveValidator>();
}

const std::shared_ptr<Chess::PieceDirector>& Chess::Chessboard::get_PieceDirector() const
{
	return m_director;
}

const std::shared_ptr<Chess::MoveValidator>& Chess::Chessboard::get_MoveValidator() const
{
	return m_validator;
}

Chess::PieceColorAndType Chess::Chessboard::GetPieceColorAndType(const Coordinate& from) const
{
	auto piece = m_director->GetPiece(from);

	if (!piece)
	{
		return PieceColorAndType();
	}

	return piece->get_ColorAndType();
}

bool Chess::Chessboard::TryInitPiece(const Coordinate& from)
{
	m_director->InitCurrantPiece(from);
	
	if (!m_director->get_CurrentPiece())
	{
		return false;
	}

	m_validator->CalculatePossibleMoves(m_director->get_CurrentPiece(), m_director->get_PiecesOnBoard());

	if (m_validator->get_PossibleMoves().size() < 1)
	{
		return false;
	}

	return true;
}

bool Chess::Chessboard::TryMovePiece(const Coordinate& to)
{
	if (!m_validator->IsValidMove(m_director->get_CurrentPiece(), to))
	{
		return false;
	}

	m_validator->ClearPossibleMoves();
	m_director->MovePiece(to);

	return true;
}
