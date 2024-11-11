#include "DrawChecker.h"

#include "Counts.h"
#include "eError.h"
#include "ErrorConverter.h"
#include "MoveValidator.h"
#include "PieceDirector.h"
#include "Sizes.h"
#include "../Chessboard.h"
#include "../pieces/logic/PieceColorAndType.h"
#include "../pieces/logic/PieceColorConverter.h"
#include "../pieces/logic/PieceFinder.h"
#include "../pieces/logic/PieceTypeConverter.h"
#include "../pieces/Pawn.h"

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
	auto blackBishopDarkCount = 0;
	auto blackBishopLightCount = 0;
	auto blackKing = false;
	auto blackKnightCount = 0;
	auto whiteBishopDarkCount = 0;
	auto whiteBishopLightCount = 0;
	auto whiteKing = false;
	auto whiteKnightCount = 0;

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
				if (colorAndType.get_Color() == ePieceColor::BLACK)
				{
					((x + y) % 2 == 0) ? ++blackBishopLightCount : ++blackBishopDarkCount;
				}
				else
				{
					((x + y) % 2 == 0) ? ++whiteBishopLightCount : ++whiteBishopDarkCount;
				}
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

	auto insufficientWhiteBishops = (whiteBishopLightCount == 0 || whiteBishopDarkCount == 0);
	auto insufficientBlackBishops = (blackBishopLightCount == 0 || blackBishopDarkCount == 0);

	if (whiteKing && blackKing
		&& (blackKnightCount + blackBishopLightCount + blackBishopDarkCount + whiteBishopLightCount + whiteBishopDarkCount + whiteKnightCount <= 1
			|| (blackKnightCount + blackBishopLightCount + blackBishopDarkCount + whiteBishopLightCount + whiteBishopDarkCount + whiteKnightCount <= 2
				&& insufficientWhiteBishops && insufficientBlackBishops)))
	{
		return true;
	}

	return false;
}

bool Chess::DrawChecker::IsDraw(const std::shared_ptr<Chessboard>& chessboard)
{
	CalculateMovesCountWithoutPawnAndTaking(chessboard);

	return !chessboard->get_MoveValidator()->GetPiecesCanMoveCount()
		|| m_movesCountWithoutPawnAndTaking >= MAX_MOVES_COUNT_WITHOUT_PAWN_MOVE_AND_TAKING
		|| IsInsufficientMaterial(chessboard);
}
