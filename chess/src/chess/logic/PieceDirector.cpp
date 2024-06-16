#include "PieceDirector.h"

#include "Counts.h"
#include "Sizes.h"
#include "../pieces/Bishop.h"
#include "../pieces/King.h"
#include "../pieces/Knight.h"
#include "../pieces/logic/ePieceColor.h"
#include "../pieces/logic/PieceColorAndType.h"
#include "../pieces/Pawn.h"
#include "../pieces/Queen.h"
#include "../pieces/Rook.h"

Chess::PieceDirector::PieceDirector()
{
	m_piecesOnBoard.reserve(MAX_COUNT_ELEMENTS);
	m_eatenPieces.reserve(MAX_COUNT_ELEMENTS);

	for (auto i = 1; i <= COUNT_OF_BISHOP_KNIGHT_ROOK_WITH_1_COLOR; ++i)
	{
		m_piecesOnBoard.emplace_back(new Bishop(ePieceColor::BLACK, i));
		m_piecesOnBoard.emplace_back(new Bishop(ePieceColor::WHITE, i));
		m_piecesOnBoard.emplace_back(new Knight(ePieceColor::BLACK, i));
		m_piecesOnBoard.emplace_back(new Knight(ePieceColor::WHITE, i));
		m_piecesOnBoard.emplace_back(new Rook(ePieceColor::BLACK, i));
		m_piecesOnBoard.emplace_back(new Rook(ePieceColor::WHITE, i));
	}

	m_piecesOnBoard.emplace_back(new King(ePieceColor::BLACK));
	m_piecesOnBoard.emplace_back(new King(ePieceColor::WHITE));

	for (auto i = 0; i < CHESSBOARD_SIZE; ++i)
	{
		m_piecesOnBoard.emplace_back(new Pawn(ePieceColor::BLACK, 'A' + i));
		m_piecesOnBoard.emplace_back(new Pawn(ePieceColor::WHITE, 'A' + i));
	}

	m_piecesOnBoard.emplace_back(new Queen(ePieceColor::BLACK));
	m_piecesOnBoard.emplace_back(new Queen(ePieceColor::WHITE));
}

const std::shared_ptr<Chess::IPiece>& Chess::PieceDirector::get_CurrentPiece() const
{
	return m_currentPiece;
}

const std::vector<std::shared_ptr<Chess::IPiece>>& Chess::PieceDirector::get_EatenPieces() const
{
	return m_eatenPieces;
}

const std::vector<std::shared_ptr<Chess::IPiece>>& Chess::PieceDirector::get_PiecesOnBoard() const
{
	return m_piecesOnBoard;
}

void Chess::PieceDirector::Take(size_t indexOnBoard)
{
	m_eatenPieces.emplace_back(std::move(m_piecesOnBoard[indexOnBoard]));
	m_piecesOnBoard.erase(m_piecesOnBoard.begin() + indexOnBoard);
}

Chess::PieceColorAndType Chess::PieceDirector::GetPieceColorAndType(const Coordinate& from) const
{
	auto piece = GetPiece(from);

	if (!piece)
	{
		return PieceColorAndType();
	}

	return piece->get_ColorAndType();
}

std::shared_ptr<Chess::IPiece> Chess::PieceDirector::GetPiece(const Coordinate& from) const
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

void Chess::PieceDirector::InitCurrantPiece(const Coordinate& from)
{
	m_currentPiece = GetPiece(from);
}

void Chess::PieceDirector::MovePiece(const Coordinate& to)
{
	auto it = std::find_if(m_piecesOnBoard.begin(), m_piecesOnBoard.end(), [to](std::shared_ptr<IPiece> current)
		{
			return current->get_Position() == to;
		});

	if (it != m_piecesOnBoard.end())
	{
		Take(std::distance(m_piecesOnBoard.begin(), it));
	}

	m_currentPiece->Move(to);
}
