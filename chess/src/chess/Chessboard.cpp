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

const std::vector<std::shared_ptr<Chess::IPiece>>& Chess::Chessboard::get_EatenPieces() const
{
	return m_eatenPieces;
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

void Chess::Chessboard::Take(int indexOnBoard)
{
	m_eatenPieces.push_back(std::move(m_piecesOnBoard[indexOnBoard]));
	m_piecesOnBoard.erase(m_piecesOnBoard.begin() + indexOnBoard);
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

bool Chess::Chessboard::IsCoordinateInPossibleMoves(Coordinate coordinate)
{
	auto it = std::find(m_possibleMoves.begin(), m_possibleMoves.end(), coordinate);

	return it != m_possibleMoves.end();
}

bool Chess::Chessboard::TryInitPiece(Coordinate from)
{
	m_carrentPiece = GetPiece(from);
	
	if (!m_carrentPiece)
	{
		return false;
	}

	m_possibleMoves = m_checker->GetPossibleMoves(m_carrentPiece, m_piecesOnBoard);

	if (m_possibleMoves.size() < 1)
	{
		return false;
	}

	return true;
}

bool Chess::Chessboard::TryMovePiece(Coordinate to)
{
	if (!IsValidMove(to))
	{
		return false;
	}

	m_possibleMoves.clear();

	auto it = std::find_if(m_piecesOnBoard.begin(), m_piecesOnBoard.end(), [to](std::shared_ptr<IPiece> current)
		{
			return current->get_Position() == to;
		});

	if (it != m_piecesOnBoard.end())
	{
		Take(std::distance(m_piecesOnBoard.begin(), it));
	}

	m_carrentPiece->Move(to);

	return true;
}
