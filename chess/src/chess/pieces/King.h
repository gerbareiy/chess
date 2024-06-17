#pragma once

#include "IPiece.h"
#include "logic/eCastleSide.h"
#include "logic/ePieceColor.h"
#include "logic/IKing.h"
#include "logic/PieceColorAndType.h"
#include "../logic/Coordinate.h"

#include <boost/signals2.hpp>

namespace Chess
{
	class King : public IPiece, public IKing
	{
	private:
		bool m_canMakeCastling = true;
		PieceColorAndType m_colorAndType{};
		bool m_isCheck = false;
		Coordinate m_position{};
		boost::signals2::signal<void(Coordinate, eCastleSide)> m_signalCastling;

	public:
		King(ePieceColor color);

	public:
		bool get_CanMakeCastling() const;
		PieceColorAndType get_ColorAndType() const override;
		bool get_IsCheck() const override;
		Coordinate get_Position() const override;

	private:
		void DisableCastling();

	public:
		boost::signals2::connection ConnectCastling(const boost::signals2::signal<void(Coordinate, eCastleSide)>::slot_type& subscriber) override;
		void Move(Coordinate to) override;
	};
}
