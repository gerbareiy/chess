#pragma once

#include "IMoveChecker.h"

#include <memory>

namespace Chess
{
    class Piece;

    class MoveChecker
    {
        std::shared_ptr<IMoveChecker> m_moveCheckerOfPiece;
        std::shared_ptr<Piece>        m_piece;

        std::vector<Coordinate> FindUncheckedMove(const Coordinate& move, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const;

    public:
        explicit MoveChecker(const std::shared_ptr<Piece>& piece);

        std::vector<Coordinate> GetFilteredMoves(const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const;
    };
} // namespace Chess
