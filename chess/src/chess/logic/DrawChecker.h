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
		// I need this for...
		std::map<std::string, int> m_boardPosition;
		int m_movesCountWithoutPawnAndTaking = 0;
		std::string m_lastBoardPosition = "";

	private:
		std::string GetLastBoardPosition(const std::shared_ptr<Chessboard>& chessboard);
		void CalculateMovesWithoutPawnAndTaking(const std::shared_ptr<Chessboard>& chessboard);

	public:
		bool IsDraw(const std::shared_ptr<Chessboard>& chessboard);
	};
}
