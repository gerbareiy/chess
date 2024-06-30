#include "DrawChecker.h"

#include "Counts.h"
#include "eError.h"
#include "ErrorConverter.h"
#include "Sizes.h"
#include "../pieces/logic/PieceColorConverter.h"
#include "../pieces/logic/PieceFinder.h"
#include "../pieces/logic/PieceTypeConverter.h"

#include <stdexcept>

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

bool Chess::DrawChecker::IsInsufficientMaterial(const std::shared_ptr<Chessboard>& chessboard)
{
	int blackBishopCount = 0;
	bool blackKing = false;
	int blackKnightCount = 0;
	int whiteBishopCount = 0;
	bool whiteKing = false;
	int whiteKnightCount = 0;

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

			switch (colorAndType.get_Type())
			{
			case ePieceType::BISHOP:
				(colorAndType.get_Color() == ePieceColor::BLACK) ? ++blackBishopCount : ++whiteBishopCount;
				break;
			case ePieceType::KNIGHT:
				(colorAndType.get_Color() == ePieceColor::BLACK) ? ++blackKnightCount : ++whiteKnightCount;
				break;
			case ePieceType::KING:
				(colorAndType.get_Color() == ePieceColor::BLACK) ? blackKing = true : whiteKing = true;
				break;
			default:
				return false;
			}
		}
	}

	if (whiteKing && blackKing
		&& blackKnightCount + blackBishopCount + whiteBishopCount + whiteKnightCount <= 1)
	{
		return true;
	}

	return false;
}

bool Chess::DrawChecker::IsDraw(const std::shared_ptr<Chessboard>& chessboard)
{
	CalculateMovesCountWithoutPawnAndTaking(chessboard);

	return !chessboard->get_MoveValidator()->GetPiecesCanMoveCount()
		|| m_movesCountWithoutPawnAndTaking >= MAX_MOVES_COUNT_WITHOUT_PAWN_MOVE_AND_TAKING;
}
