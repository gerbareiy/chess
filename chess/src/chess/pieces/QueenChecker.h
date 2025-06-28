#pragma once

#include "Queen.h"
#include "logic/IMoveChecker.h"

#include <vector>

namespace Chess
{
    struct Coordinate;

    class QueenChecker final : public IMoveChecker
    {
    private:
        static std::vector<Coordinate> FindPossibleMoves(const std::shared_ptr<Queen>& queen, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard);

    public:
        virtual std::vector<Coordinate> GetMoves(const std::shared_ptr<Piece>& piece, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const override;
    };
} // namespace Chess
