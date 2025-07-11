#pragma once

#include "core/logic/Coordinate.h"

#include <memory>
#include <unordered_map>
#include <vector>

namespace Chess
{
    class Piece;
    struct Coordinate;

    class PieceFinder
    {
        std::unordered_map<Coordinate, std::shared_ptr<Piece>> m_pieceMap;

    public:
        explicit PieceFinder(const std::vector<std::shared_ptr<Piece>>& pieces);

        std::shared_ptr<Piece> Find(const Coordinate& coordinate) const;
    };
} // namespace Chess
