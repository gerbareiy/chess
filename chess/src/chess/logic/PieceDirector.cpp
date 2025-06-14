#include "PieceDirector.h"

#include "Coordinate.h"
#include "Counts.h"
#include "Sizes.h"
#include "PieceSignalDirector.h"
#include "PieceTakeLocator.h"
#include "Promotion.h"
#include "../pieces/logic/CheckChecker.h"
#include "../pieces/logic/ePieceColor.h"
#include "../pieces/logic/PieceColorAndType.h"
#include "../pieces/Pawn.h"
#include "../pieces/Piece.h"

Chess::PieceDirector::PieceDirector(std::vector<std::shared_ptr<Piece>>& piecesOnBoard, std::shared_ptr<PieceSignalDirector> const& signalDirector)
	: m_piecesOnBoard(piecesOnBoard), m_signalDirector(signalDirector)
{
	m_eatenPieces.reserve(MAX_COUNT_ELEMENTS);
	m_promotion = std::make_shared<Promotion>();
}

void Chess::PieceDirector::Take(int indexOnBoard)
{
	m_eatenPieces.emplace_back(std::move(m_piecesOnBoard[indexOnBoard]));
	m_piecesOnBoard.erase(m_piecesOnBoard.begin() + indexOnBoard);
}

std::shared_ptr<Chess::Piece> const& Chess::PieceDirector::GetCurrentPiece() const
{
	return m_currentPiece;
}

std::vector<std::shared_ptr<Chess::Piece>> const& Chess::PieceDirector::GetEatenPieces() const
{
	return m_eatenPieces;
}

bool Chess::PieceDirector::GetIsCheck() const
{
	return m_isCheck;
}

std::vector<std::shared_ptr<Chess::Piece>> const& Chess::PieceDirector::GetPiecesOnBoard() const
{
	return m_piecesOnBoard;
}

Chess::PieceColorAndType Chess::PieceDirector::GetPieceColorAndType(Coordinate const& from) const
{
	auto piece = GetPiece(from);

	if (!piece)
	{
		return PieceColorAndType();
	}

	return piece->GetColorAndType();
}

std::shared_ptr<Chess::Piece> Chess::PieceDirector::GetPiece(Coordinate const& from) const
{
	for (std::shared_ptr<Chess::Piece> pieceOnBoard : m_piecesOnBoard)
	{
		if (pieceOnBoard->GetPosition() == from)
		{
			return pieceOnBoard;
		}
	}

	return nullptr;
}

void Chess::PieceDirector::InitCurrentPiece(Coordinate const& from)
{
	m_currentPiece = GetPiece(from);
}

void Chess::PieceDirector::MovePiece(Coordinate const& to, boost::signals2::signal<void()> const& signalChessboardUndated)
{
	auto fromTake = std::make_unique<PieceTakeLocator>()->Find(m_currentPiece, m_piecesOnBoard, to);

	auto it = std::find_if(m_piecesOnBoard.begin(), m_piecesOnBoard.end(), [fromTake](std::shared_ptr<Piece> const& current)
		{
			return current->GetPosition() == fromTake;
		});

	if (it != m_piecesOnBoard.end())
	{
		Take(static_cast<int>(std::distance(m_piecesOnBoard.begin(), it)));
	}

	m_currentPiece->Move(to);
	m_signalDirector->Invite();

	if (typeid(*m_currentPiece) == typeid(Pawn)
		&& (m_currentPiece->GetPosition().GetRank() == 1 && m_currentPiece->GetColorAndType().GetColor() == ePieceColor::BLACK
			|| m_currentPiece->GetPosition().GetRank() == CHESSBOARD_SIZE && m_currentPiece->GetColorAndType().GetColor() == ePieceColor::WHITE))
	{
		signalChessboardUndated();
		m_promotion->PromoteConditionally(std::static_pointer_cast<Pawn>(m_currentPiece), m_piecesOnBoard);
	}

	auto checkChecker = std::make_unique<CheckChecker>();

	ePieceColor color;

	if (m_currentPiece->GetColorAndType().GetColor() == ePieceColor::BLACK)
	{
		color = ePieceColor::WHITE;
	}
	else if (m_currentPiece->GetColorAndType().GetColor() == ePieceColor::WHITE)
	{
		color = ePieceColor::BLACK;
	}

	m_isCheck = checkChecker->IsCheck(color, m_piecesOnBoard);
	m_signalDirector->Invite(m_isCheck);
}
