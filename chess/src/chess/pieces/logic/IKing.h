#pragma once

#include "eCastleSide.h"
#include "ICastlable.h"
#include "../IPiece.h"
#include "../../logic/Coordinate.h"

#include "boost/signals2.hpp"

namespace Chess
{
	class IKing : public ICastable, public IPiece
	{
	public:
		virtual ~IKing() = default;

	public:
		virtual bool get_CanMakeCastling() const = 0;
		virtual PieceColorAndType get_ColorAndType() const = 0;
		virtual bool get_IsCheck() const = 0;
		virtual Coordinate get_Position() const = 0;

	public:
		virtual boost::signals2::connection ConnectCastling(const boost::signals2::signal<void(Coordinate, eCastleSide)>::slot_type& subscriber) = 0;
		virtual void Move(Coordinate to) = 0;
	};
}
