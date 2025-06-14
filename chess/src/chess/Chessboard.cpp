#include "Chessboard.h"

#include "logic/Coordinate.h"
#include "pieces/logic/ePieceColor.h"
#include "pieces/Piece.h"
#include "pieces/King.h"
#include "logic/MoveValidator.h"
#include "logic/PieceDirector.h"
#include "logic/PieceSignalDirector.h"
#include "Player.h"

Chess::Chessboard::Chessboard(std::vector<std::shared_ptr<Piece>> const& piecesOnBoard, std::shared_ptr<PieceSignalDirector> signalDirector)
{
	m_piecesOnBoard = piecesOnBoard;
	m_director = std::make_shared<PieceDirector>(m_piecesOnBoard, signalDirector);
	m_validator = std::make_shared<MoveValidator>(m_piecesOnBoard, std::make_shared<Player>(ePieceColor::WHITE, signalDirector));
}

Chess::Coordinate Chess::Chessboard::GetFrom() const
{
	return m_from;
}

std::shared_ptr<Chess::MoveValidator> const& Chess::Chessboard::GetMoveValidator() const
{
	return m_validator;
}

std::shared_ptr<Chess::PieceDirector> const& Chess::Chessboard::GetPieceDirector() const
{
	return m_director;
}

Chess::Coordinate Chess::Chessboard::GetTo() const
{
	return m_to;
}

bool Chess::Chessboard::TryInitPiece(Coordinate const& from)
{
	m_from = from;
	m_to = Coordinate(0, 0);
	m_director->InitCurrentPiece(from);

	if (!m_director->GetCurrentPiece())
	{
		return false;
	}

	m_validator->CalculatePossibleMoves(m_director->GetCurrentPiece());

	if (m_validator->GetPossibleMoves().size() < 1)
	{
		return false;
	}

	m_signalChessboardUndated();

	return true;
}

bool Chess::Chessboard::TryMovePiece(Coordinate const& to)
{
	m_to = to;

	if (!m_validator->IsValidMove(m_director->GetCurrentPiece(), to))
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

boost::signals2::connection Chess::Chessboard::ConnectChessboardUpdated(boost::signals2::signal<void()>::slot_type const& subscriber)
{
	return m_signalChessboardUndated.connect(subscriber);
}
