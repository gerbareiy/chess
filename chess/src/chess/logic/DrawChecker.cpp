#include "DrawChecker.h"

#include "Counts.h"
#include "Sizes.h"
#include "../pieces/logic/PieceColorConverter.h"
#include "../pieces/logic/PieceFinder.h"
#include "../pieces/logic/PieceTypeConverter.h"

void Chess::DrawChecker::CalculateMovesCountWithoutPawnAndTaking(const std::shared_ptr<Chessboard>& chessboard)
{
	auto finder = std::make_shared<PieceFinder>(chessboard->get_PieceDirector()->get_PiecesOnBoard());
	auto piece = finder->Find(chessboard->get_To());

	if (!piece)
	{
		return;
	}

	auto eatenPiecesCount = chessboard->get_PieceDirector()->get_EatenPieces().size();

	if (typeid(*piece) == typeid(Pawn) || m_lastCountEatenPeaces != eatenPiecesCount)
	{
		m_movesCountWithoutPawnAndTaking = 0;
		m_lastCountEatenPeaces = eatenPiecesCount;
	}
	else
	{
		++m_movesCountWithoutPawnAndTaking;
	}
}

bool Chess::DrawChecker::IsDraw(const std::shared_ptr<Chessboard>& chessboard)
{
	CalculateMovesCountWithoutPawnAndTaking(chessboard);
	
	return !chessboard->get_MoveValidator()->GetPiecesCanMoveCount()
		|| m_movesCountWithoutPawnAndTaking >= MAX_MOVES_COUNT_WITHOUT_PAWN_MOVE_AND_TAKING;
}
