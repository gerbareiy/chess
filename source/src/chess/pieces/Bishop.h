#pragma once

#include "Piece.h"
#include "logic/ePieceColor.h"

namespace Chess
{
    struct Coordinate;
    struct PieceColorAndType;

    class Bishop final : public Piece
    {
    public:
        Bishop(ePieceColor color, int orderNumber);
        Bishop(ePieceColor color, const Coordinate& coordinate);
    };
} // namespace Chess
