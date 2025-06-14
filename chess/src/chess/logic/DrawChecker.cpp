#include "DrawChecker.h"

#include "Counts.h"
#include "MoveValidator.h"
#include "PieceDirector.h"
#include "Sizes.h"
#include "../Chessboard.h"
#include "../pieces/logic/PieceColorAndType.h"
#include "../pieces/logic/PieceColorConverter.h"
#include "../pieces/logic/PieceFinder.h"
#include "../pieces/logic/PieceTypeConverter.h"
#include "../pieces/Pawn.h"

void Chess::DrawChecker::CalculateMovesCountWithoutPawnAndTaking(std::shared_ptr<Chessboard> const& chessboard)
{
	auto const finder = PieceFinder(chessboard->GetPieceDirector()->GetPiecesOnBoard());
	auto const& piece = finder.Find(chessboard->GetTo());

	if (!piece)
	{
		return;
	}

	auto const eatenPiecesCount = chessboard->GetPieceDirector()->GetEatenPieces().size();

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

bool Chess::DrawChecker::IsInsufficientMaterial(std::shared_ptr<Chessboard> const& chessboard)
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
			auto piece = chessboard->GetPieceDirector()->GetPiece(Coordinate(x, y));

			if (!piece)
			{
				continue;
			}

			auto colorAndType = piece->GetColorAndType();

			switch (colorAndType.GetType())
			{
			case ePieceType::BISHOP:
				if (colorAndType.GetColor() == ePieceColor::BLACK)
				{
					((x + y) % 2 == 0) ? ++blackBishopLightCount : ++blackBishopDarkCount;
				}
				else
				{
					((x + y) % 2 == 0) ? ++whiteBishopLightCount : ++whiteBishopDarkCount;
				}
				break;
			case ePieceType::KNIGHT:
				(colorAndType.GetColor() == ePieceColor::BLACK) ? ++blackKnightCount : ++whiteKnightCount;
				break;
			case ePieceType::KING:
				(colorAndType.GetColor() == ePieceColor::BLACK) ? blackKing = true : whiteKing = true;
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

bool Chess::DrawChecker::IsDraw(std::shared_ptr<Chessboard> const& chessboard)
{
	CalculateMovesCountWithoutPawnAndTaking(chessboard);

	return !chessboard->GetMoveValidator()->GetPiecesCanMoveCount()
		|| m_movesCountWithoutPawnAndTaking >= MAX_MOVES_COUNT_WITHOUT_PAWN_MOVE_AND_TAKING
		|| IsInsufficientMaterial(chessboard);
}
