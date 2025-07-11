#pragma once

#include "core/logic/Coordinate.h"
#include "logic/PieceColorAndType.h"

namespace Chess
{
    class Piece
    {
    protected:
        PieceColorAndType m_colorAndType;
        Coordinate        m_position;

    public:
        Piece();
        Piece(const PieceColorAndType& colorAndType, const Coordinate& coordinate);
        virtual ~Piece() = default;

        PieceColorAndType GetColorAndType() const;
        Coordinate        GetPosition() const;

        virtual void Move(Coordinate to, bool isRealMove = true);
    };
} // namespace Chess
