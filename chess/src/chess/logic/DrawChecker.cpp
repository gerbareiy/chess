#include "DrawChecker.h"

#include "Counts.h"
#include "Sizes.h"
#include "../pieces/logic/PieceColorConverter.h"
#include "../pieces/logic/PieceFinder.h"
#include "../pieces/logic/PieceTypeConverter.h"

std::string Chess::DrawChecker::GetLastBoardPosition(const std::shared_ptr<Chessboard>& chessboard)
{
	std::string chessboardPosition = "";
	auto finder = std::make_shared<PieceFinder>(chessboard->get_PieceDirector()->get_PiecesOnBoard());

	for (auto y = CHESSBOARD_SIZE; y > 0; --y)
	{
		for (auto x = 'A'; x < 'A' + CHESSBOARD_SIZE; ++x)
		{
			auto piece = chessboard->get_PieceDirector()->GetPiece(Coordinate(x, y));

			if (!piece)
			{
				continue;
			}

			auto colorAndType = piece->get_ColorAndType();

			chessboardPosition += PieceColorConverter::ConvertToString(colorAndType.get_Color()) + PieceTypeConverter::ConvertToString(colorAndType.get_Type());
		}
	}

	return chessboardPosition;
}

void Chess::DrawChecker::CalculateMovesCountWithoutPawnAndTaking(const std::shared_ptr<Chessboard>& chessboard)
{
	auto finder = std::make_shared<PieceFinder>(chessboard->get_PieceDirector()->get_PiecesOnBoard());
	auto piece = finder->Find(chessboard->get_To());

	if (!piece)
	{
		return;
	}

	auto eatenPiecesCount = chessboard->get_PieceDirector()->get_EatenPieces().size();

	if (typeid(*piece) == typeid(Pawn))
	{
		m_movesCountWithoutPawnAndTaking = 0;
	}
	if (m_lastCountEatenPeaces != eatenPiecesCount)
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
		|| m_movesCountWithoutPawnAndTaking >= MAX_MOVES_COUNT_WITHOUT_PAWN_MOVE_AND_TAKING
		|| ++m_boardPosition[GetLastBoardPosition(chessboard)] >= MAX_COUNT_SAME_BOARD_POSITION;
}
