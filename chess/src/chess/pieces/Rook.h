#pragma once

#include "Piece.h"
#include "logic/ICastlable.h"
#include "logic/eCastleSide.h"
#include "logic/ePieceColor.h"
#include <boost/signals2.hpp>

namespace Chess
{
    class King;
    struct Coordinate;

    class Rook final : public Piece, public ICastable
    {
        bool                        m_canMakeCastling = false;
        boost::signals2::connection m_castlingConnection;

        void DisableCastling();
        void MakeTracking(const std::shared_ptr<King>& king);
        void OnCastling(const Coordinate& to, eCastleSide side);

    public:
        Rook(ePieceColor color, int orderNumber);
        Rook(ePieceColor pieceColor, int orderNumber, const std::shared_ptr<King>& king);
        Rook(ePieceColor color, const Coordinate& coordinate);
        Rook(ePieceColor color, Coordinate coordinate, const std::shared_ptr<King>& king);

        virtual bool GetCanMakeCastling() const override;

        virtual void Move(Coordinate to, bool isRealMove = true) override;
    };
} // namespace Chess
