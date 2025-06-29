#pragma once

#include <map>
#include <memory>
#include <vector>

namespace Chess
{
    class Piece;
    struct Coordinate;

    class PieceFinder
    {
        std::map<Coordinate, std::shared_ptr<Piece>> m_pieceMap;

    public:
        explicit PieceFinder(const std::vector<std::shared_ptr<Piece>>& pieces);

        std::shared_ptr<Piece> Find(const Coordinate& coordinate) const;
    };
} // namespace Chess
