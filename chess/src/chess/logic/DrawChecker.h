#pragma once

#include "../Chessboard.h"

#include <map>
#include <memory>
#include <string>

namespace Chess
{
	class DrawChecker
	{
	private:
		int m_movesCountWithoutPawnAndTaking = 0;
		int m_lastCountEatenPeaces = 0;

	private:
		std::string GetLastBoardPosition(const std::shared_ptr<Chessboard>& chessboard);
		void CalculateMovesCountWithoutPawnAndTaking(const std::shared_ptr<Chessboard>& chessboard);

	public:
		bool IsDraw(const std::shared_ptr<Chessboard>& chessboard);
	};
}
