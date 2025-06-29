#pragma once

#include "logic/IMoveChecker.h"

#include <memory>
#include <vector>

namespace Chess
{
    class Piece;
    class Knight;
    struct Coordinate;

    class KnightChecker final : public IMoveChecker
    {
        const std::vector<std::pair<int, int>> m_knightMoveDirections = {
            { 2, 1 }, { 2, -1 }, { -2, 1 }, { -2, -1 }, { 1, 2 }, { 1, -2 }, { -1, 2 }, { -1, -2 }
        };

        std::vector<Coordinate> FindPossibleMoves(const std::shared_ptr<Knight>& knight, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const;

    public:
        virtual std::vector<Coordinate> GetMoves(const std::shared_ptr<Piece>& piece, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const override;
    };
} // namespace Chess
