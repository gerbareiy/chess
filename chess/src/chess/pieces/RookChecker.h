#pragma once

#include "logic/IMoveChecker.h"

namespace Chess
{
    class Rook;

    class RookChecker final : public IMoveChecker
    {
        static std::vector<Coordinate> FindPossibleMoves(const std::shared_ptr<Rook>& rook, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard);

    public:
        virtual std::vector<Coordinate> GetMoves(const std::shared_ptr<Piece>& piece, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const override;
    };
} // namespace Chess
