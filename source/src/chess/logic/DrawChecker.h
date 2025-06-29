#pragma once

#include <memory>

namespace Chess
{
    class Chessboard;

    class DrawChecker
    {
        int    m_movesCountWithoutPawnAndTaking = 0;
        size_t m_lastCountEatenPeaces           = 0;

        void        CalculateMovesCountWithoutPawnAndTaking(const std::shared_ptr<Chessboard>& chessboard);
        static bool IsInsufficientMaterial(const std::shared_ptr<Chessboard>& chessboard);

    public:
        bool IsDraw(const std::shared_ptr<Chessboard>& chessboard);
    };
} // namespace Chess
