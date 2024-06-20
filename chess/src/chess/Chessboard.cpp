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
#include "logic/PieceInitializer.h"
#include "logic/PieceSignalDirector.h"

Chess::Chessboard::Chessboard()
{
	auto signalDirector = std::make_shared<PieceSignalDirector>();
	auto pieces = std::make_unique<PieceInitializer>()->InitStandartBoard(signalDirector);
	m_director = std::make_shared<PieceDirector>(pieces, signalDirector);
	m_validator = std::make_shared<MoveValidator>(pieces);
}

const std::shared_ptr<Chess::PieceDirector>& Chess::Chessboard::get_PieceDirector() const
{
	return m_director;
}

const std::shared_ptr<Chess::MoveValidator>& Chess::Chessboard::get_MoveValidator() const
{
	return m_validator;
}

bool Chess::Chessboard::TryInitPiece(const Coordinate& from)
{
	m_director->InitCurrentPiece(from);
	
	if (!m_director->get_CurrentPiece())
	{
		return false;
	}

	m_validator->CalculatePossibleMoves(m_director->get_CurrentPiece());

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
