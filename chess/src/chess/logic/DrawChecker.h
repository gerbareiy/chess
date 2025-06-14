#pragma once

#include <memory>

namespace Chess
{
	class Chessboard;

	class DrawChecker
	{
	private:
		int m_movesCountWithoutPawnAndTaking = 0;
		size_t m_lastCountEatenPeaces = 0;

	private:
		void CalculateMovesCountWithoutPawnAndTaking(std::shared_ptr<Chessboard> const& chessboard);
		bool IsInsufficientMaterial(std::shared_ptr<Chessboard> const& chessboard);

	public:
		bool IsDraw(std::shared_ptr<Chessboard> const& chessboard);
	};
}
