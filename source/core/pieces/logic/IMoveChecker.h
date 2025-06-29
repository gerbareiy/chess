#pragma once

#include <memory>
#include <vector>

namespace Chess
{
    class Piece;
    struct Coordinate;

    class IMoveChecker
    {
    public:
        virtual ~IMoveChecker() = default;

        virtual std::vector<Coordinate> GetMoves(const std::shared_ptr<Piece>& piece, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const = 0;
    };
} // namespace Chess
