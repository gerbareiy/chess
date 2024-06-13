#include "Chessboard.h"

#include "Counts.h"
#include "pieces/Bishop.h"
#include "pieces/ePieceColor.h"
#include "pieces/IPiece.h"
#include "pieces/King.h"
#include "pieces/Knight.h"
#include "pieces/Pawn.h"
#include "pieces/Queen.h"
#include "pieces/Rook.h"
#include "Sizes.h"

Chess::Chessboard::Chessboard()
{
	m_checker = std::make_shared<MoveChecker>();

	m_piecesOnBoard.reserve(MAX_COUNT_ELEMENTS);
	m_eatenPieces.reserve(MAX_COUNT_ELEMENTS);

	for (auto i = 1; i <= COUNT_OF_BISHOP_KNIGHT_ROOK_WITH_1_COLOR; ++i)
	{
		m_piecesOnBoard.push_back(std::make_shared<Bishop>(ePieceColor::BLACK, i));
		m_piecesOnBoard.push_back(std::make_shared<Bishop>(ePieceColor::WHITE, i));
		m_piecesOnBoard.push_back(std::make_shared<Knight>(ePieceColor::BLACK, i));
		m_piecesOnBoard.push_back(std::make_shared<Knight>(ePieceColor::WHITE, i));
		m_piecesOnBoard.push_back(std::make_shared<Rook>(ePieceColor::BLACK, i));
		m_piecesOnBoard.push_back(std::make_shared<Rook>(ePieceColor::WHITE, i));
	}

	m_piecesOnBoard.push_back(std::make_shared<King>(ePieceColor::BLACK));
	m_piecesOnBoard.push_back(std::make_shared<King>(ePieceColor::WHITE));

	for (auto i = 0; i < CHESSBOARD_SIZE; ++i)
	{
		m_piecesOnBoard.push_back(std::make_shared<Pawn>(ePieceColor::BLACK, 'A' + i));
		m_piecesOnBoard.push_back(std::make_shared<Pawn>(ePieceColor::WHITE, 'A' + i));
	}

	m_piecesOnBoard.push_back(std::make_shared<Queen>(ePieceColor::BLACK));
	m_piecesOnBoard.push_back(std::make_shared<Queen>(ePieceColor::WHITE));
}

std::shared_ptr<Chess::IPiece> Chess::Chessboard::GetPiece(Coordinate from) const
{
	for (std::shared_ptr<Chess::IPiece> pieceOnBoard : m_piecesOnBoard)
	{
		if (pieceOnBoard->get_Position() == from)
		{
			return pieceOnBoard;
		}
	}

	return nullptr;
}

bool Chess::Chessboard::IsValidMove(Coordinate to)
{
	if (!m_carrentPiece)
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

Chess::PieceColorAndType Chess::Chessboard::GetPieceColorAndType(Coordinate from) const
{
	auto piece = GetPiece(from);

	if (!piece)
	{
		return PieceColorAndType();
	}

	return piece->get_ColorAndType();
}

bool Chess::Chessboard::TryInitPiece(Coordinate from)
{
	if (m_carrentPiece = GetPiece(from))
	{
		m_possibleMoves = m_checker->GetPossibleMoves(m_carrentPiece, m_piecesOnBoard);
		return true;
	}

	return false;
}

bool Chess::Chessboard::IsCoordinateInPossibleMoves(Coordinate coordinate)
{
	auto it = std::find(m_possibleMoves.begin(), m_possibleMoves.end(), coordinate);
	return it != m_possibleMoves.end();
}

bool Chess::Chessboard::TryMovePiece(Coordinate to)
{
	if (!IsValidMove(to))
	{
		return false;
	}

	m_carrentPiece->Move(to);

	return true;
}
