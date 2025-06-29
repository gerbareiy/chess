#pragma once

#include "Piece.h"
#include "logic/ePieceColor.h"

namespace Chess
{
    struct Coordinate;

    class Queen final : public Piece
    {
    public:
        Queen(ePieceColor color);
        Queen(ePieceColor color, const Coordinate& coordinate);
    };
} // namespace Chess
