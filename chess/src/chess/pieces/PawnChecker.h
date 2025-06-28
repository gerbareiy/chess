#pragma once

#include "logic/IMoveChecker.h"

#include <memory>
#include <vector>

namespace Chess
{
    class Pawn;
    class Piece;
    class PieceFinder;

    class PawnChecker final : public IMoveChecker
    {
    private:
        static std::vector<Coordinate> GetForwardMoves(const std::shared_ptr<Pawn>&               pawn,
                                                       const std::shared_ptr<PieceFinder>&        finder);
        static std::vector<Coordinate> GetDiagonalMoves(const std::shared_ptr<Pawn>&               pawn,
                                                        const std::shared_ptr<PieceFinder>&        finder);
        static void                    ValidatePawn(const std::shared_ptr<Pawn>& pawn);

    public:
        virtual std::vector<Coordinate> GetMoves(const std::shared_ptr<Piece>& piece, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const override;
    };
} // namespace Chess
