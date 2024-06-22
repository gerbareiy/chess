#pragma once

#include "logic/eCastleSide.h"
#include "logic/ePieceColor.h"
#include "logic/IKing.h"
#include "logic/PieceColorAndType.h"
#include "../logic/PieceSignalDirector.h"
#include "../logic/Coordinate.h"

#include <boost/signals2.hpp>

#include <memory>

namespace Chess
{
	class King : public IKing
	{
	private:
		bool m_canMakeCastling = true;
		PieceColorAndType m_colorAndType;
		bool m_isCheck = false;
		Coordinate m_position;

	private:
		boost::signals2::signal<void(Coordinate, eCastleSide)> m_signalCastling;

	public:
		King(ePieceColor color);
		King(ePieceColor color, const std::shared_ptr<PieceSignalDirector>& signalDirector);
		King(ePieceColor color, Coordinate coordinate);
		King(ePieceColor color, Coordinate coordinate, const std::shared_ptr<PieceSignalDirector>& signalDirector);

	public:
		bool get_CanMakeCastling() const;
		PieceColorAndType get_ColorAndType() const override;
		bool get_IsCheck() const override;
		Coordinate get_Position() const override;

	private:
		void DisableCastling();

	public:
		void Move(Coordinate to) override;

	public:
		boost::signals2::connection ConnectCastling(const boost::signals2::signal<void(Coordinate, eCastleSide)>::slot_type& subscriber) override;
	};
}
