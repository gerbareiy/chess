#pragma once

#include "logic/IMoveChecker.h"

#include <memory>
#include <vector>

namespace Chess
{
    class Piece;
    struct Coordinate;

    class KnightChecker final : public IMoveChecker
    {
        std::vector<std::pair<int, int>> m_knightMoveDirections = { { 2, 1 }, { 2, -1 }, { -2, 1 }, { -2, -1 }, { 1, 2 }, { 1, -2 }, { -1, 2 }, { -1, -2 } };

        std::vector<Coordinate> FindPossibleMoves(const std::shared_ptr<Piece>& knight, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const;

    public:
        virtual std::vector<Coordinate> GetMoves(const std::shared_ptr<Piece>& piece, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const override;
    };
} // namespace Chess
