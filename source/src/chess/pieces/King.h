#pragma once

#include "Piece.h"
#include "logic/ICastlable.h"
#include "logic/eCastleSide.h"
#include "logic/ePieceColor.h"
#include <boost/signals2.hpp>

#include <memory>

namespace Chess
{
    class PieceSignalDirector;
    struct Coordinate;
    struct PieceColorAndType;

    class King final : public ICastable, public Piece
    {
        bool m_canMakeCastling = true;
        bool m_isCheck         = false;

        boost::signals2::signal<void(Coordinate, eCastleSide)> m_signalCastling;

        void DisableCastling();
        void MakeTracking(const std::shared_ptr<PieceSignalDirector>& signalDirector);

    public:
        King(ePieceColor color);
        King(ePieceColor color, const std::shared_ptr<PieceSignalDirector>& signalDirector);
        King(ePieceColor color, const Coordinate& coordinate);
        King(ePieceColor color, const Coordinate& coordinate, const std::shared_ptr<PieceSignalDirector>& signalDirector);

        virtual bool GetCanMakeCastling() const override;
        bool         GetIsCheck() const;

        virtual void Move(Coordinate to, bool isRealMove = true) override;

        boost::signals2::connection ConnectCastling(const boost::signals2::signal<void(Coordinate, eCastleSide)>::slot_type& subscriber);
    };
} // namespace Chess
