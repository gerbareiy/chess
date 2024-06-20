#include "CheckChecker.h"

Chess::CheckChecker::CheckChecker(std::shared_ptr<MoveChecker> moveChecker) : m_moveChecker(moveChecker) { }

bool Chess::CheckChecker::IsCheck(std::shared_ptr<IKing> king, const std::vector<std::shared_ptr<IPiece>>& piecesOnBoard)
{
	for (const auto& piece : piecesOnBoard)
	{
		if (piece->get_ColorAndType().get_Color() != king->get_ColorAndType().get_Color())
		{
			auto moves = m_moveChecker->GetPossibleMoves(piece, piecesOnBoard);

			for (const auto& move : moves)
			{
				if (move == king->get_Position())
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool Chess::CheckChecker::IsCheck(const Chess::ePieceColor& kingColor, const std::vector<std::shared_ptr<IPiece>>& piecesOnBoard)
{
	for (const auto& piece : piecesOnBoard)
	{
		if (auto king = std::dynamic_pointer_cast<IKing>(piece))
		{
			if (king->get_ColorAndType().get_Color() == kingColor)
			{
				return IsCheck(king, piecesOnBoard);
			}
		}
	}

	return false;
}
