#include "CheckChecker.h"

#include "MoveCheckerFactory.h"

bool Chess::CheckChecker::IsCheck(std::shared_ptr<IKing> king, const std::vector<std::shared_ptr<IPiece>>& piecesOnBoard) const
{
	for (const auto& piece : piecesOnBoard)
	{
		if (piece->get_ColorAndType().get_Color() == king->get_ColorAndType().get_Color())
		{
			continue;
		}
		
		auto moveChecker = std::make_unique<MoveCheckerFactory>()->Create(piece);
		auto moves = moveChecker->GetMoves(piece, piecesOnBoard);

		for (const auto& move : moves)
		{
			if (move == king->get_Position())
			{
				return true;
			}
		}
	}

	return false;
}

bool Chess::CheckChecker::IsCheck(const Chess::ePieceColor& kingColor, const std::vector<std::shared_ptr<IPiece>>& piecesOnBoard) const
{
	for (const auto& piece : piecesOnBoard)
	{
		auto king = std::dynamic_pointer_cast<IKing>(piece);

		if (king && king->get_ColorAndType().get_Color() == kingColor)
		{
			return IsCheck(king, piecesOnBoard);
		}
	}

	return false;
}
