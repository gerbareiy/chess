#pragma once

#include "Piece.h"
#include "logic/ePieceColor.h"

#include "memory"

namespace Chess
{
    class PieceSignalDirector;
    struct Coordinate;
    struct PieceColorAndType;

    class Pawn final : public Piece
    {
        static constexpr int m_MAX_POSSIBLE_COUNT_MOVES = 2;

        bool m_canEnPassant      = false;
        bool m_isOnPawnFirstMove = false;
        bool m_isNotMoved        = true;

        void LostEnPassant();
        void MakeTracking(const std::shared_ptr<PieceSignalDirector>& signalDirector);

    public:
        Pawn(ePieceColor color, char file);
        Pawn(ePieceColor color, char file, const std::shared_ptr<PieceSignalDirector>& signalDirector);
        Pawn(ePieceColor color, const Coordinate& coordinate);
        Pawn(ePieceColor color, const Coordinate& coordinate, const std::shared_ptr<PieceSignalDirector>& signalDirector);

        bool GetCanEnPassant() const;
        bool GetIsNotMoved() const;

        virtual void Move(Coordinate to, bool isRealMove = true) override;
    };
} // namespace Chess
