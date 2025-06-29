#pragma once

#include <memory>
#include <vector>

namespace Chess
{
    class Pawn;
    class Piece;
    struct Coordinate;

    class Promotion
    {
    public:
        static void PromoteConditionally(const std::shared_ptr<Pawn>& pawn, std::vector<std::shared_ptr<Piece>>& piecesOnBoard);
        static void PromoteConditionally(const Coordinate& pawnPosition, std::vector<std::shared_ptr<Piece>>& piecesOnBoard);
    };
} // namespace Chess
