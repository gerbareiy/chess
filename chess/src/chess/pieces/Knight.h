#pragma once

#include "Piece.h"
#include "logic/ePieceColor.h"

namespace Chess
{
    struct Coordinate;
    struct PieceColorAndType;

    class Knight final : public Piece
    {
    public:
        Knight(ePieceColor pieceColor, int orderNumber);
        Knight(ePieceColor color, const Coordinate& coordinate);
    };
} // namespace Chess
