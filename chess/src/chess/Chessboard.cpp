#include "Chessboard.h"

#include "logic/Coordinate.h"
#include "logic/Counts.h"
#include "pieces/Bishop.h"
#include "pieces/logic/ePieceColor.h"
#include "pieces/Piece.h"
#include "pieces/King.h"
#include "pieces/Knight.h"
#include "pieces/Pawn.h"
#include "pieces/Queen.h"
#include "pieces/Rook.h"
#include "logic/MoveValidator.h"
#include "logic/Sizes.h"
#include "logic/PieceDirector.h"
#include "logic/PieceSignalDirector.h"
#include "pieces/logic/PieceColorAndType.h"
#include "pieces/logic/PositionChecker.h"
#include "Player.h"

Chess::Chessboard::Chessboard(std::vector<std::shared_ptr<Piece>> piecesOnBoard, std::shared_ptr<PieceSignalDirector> signalDirector)
{
	m_piecesOnBoard = piecesOnBoard;
	m_director = std::make_shared<PieceDirector>(m_piecesOnBoard, signalDirector);
	m_validator = std::make_shared<MoveValidator>(m_piecesOnBoard, std::make_shared<Player>(ePieceColor::WHITE, signalDirector));
}

Chess::Coordinate Chess::Chessboard::get_From() const
{
	return m_from;
}

const std::shared_ptr<Chess::MoveValidator>& Chess::Chessboard::get_MoveValidator() const
{
	return m_validator;
}

const std::shared_ptr<Chess::PieceDirector>& Chess::Chessboard::get_PieceDirector() const
{
	return m_director;
}

Chess::Coordinate Chess::Chessboard::get_To() const
{
	return m_to;
}

bool Chess::Chessboard::TryInitPiece(const Coordinate& from)
{
	m_from = from;
	m_to = Coordinate(0, 0);
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

	m_signalChessboardUndated();

	return true;
}

bool Chess::Chessboard::TryMovePiece(const Coordinate& to)
{
	m_to = to;

	if (!m_validator->IsValidMove(m_director->get_CurrentPiece(), to))
	{
		return false;
	}

	m_validator->ClearPossibleMoves();
	m_validator->ClearPiecesCanMove();
	m_director->MovePiece(to, m_signalChessboardUndated);
	m_validator->CalculatePiecesCanMove();

	m_signalChessboardUndated();

	return true;
}

boost::signals2::connection Chess::Chessboard::ConnectChessboardUpdated(const boost::signals2::signal<void()>::slot_type& subscriber)
{
	return m_signalChessboardUndated.connect(subscriber);
}
