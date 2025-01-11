#pragma once

#include "logic/eCastleSide.h"
#include "logic/ePieceColor.h"
#include "logic/ICastlable.h"
#include "Piece.h"

#include <boost/signals2.hpp>

#include <memory>

namespace Chess
{
	class PieceSignalDirector;
	struct Coordinate;
	struct PieceColorAndType;

	class King : public ICastable, public Piece
	{
	private:
		bool m_canMakeCastling = true;
		bool m_isCheck = false;

	private:
		boost::signals2::signal<void(Coordinate, eCastleSide)> m_signalCastling;

	public:
		King(ePieceColor color);
		King(ePieceColor color, const std::shared_ptr<PieceSignalDirector>& signalDirector);
		King(ePieceColor color, Coordinate coordinate);
		King(ePieceColor color, Coordinate coordinate, const std::shared_ptr<PieceSignalDirector>& signalDirector);

	public:
		bool GetCanMakeCastling() const;
		bool GetIsCheck() const;

	private:
		void DisableCastling();
		void MakeTracking(const std::shared_ptr<Chess::PieceSignalDirector>& signalDirector);

	public:
		void Move(Coordinate to, bool isRealMove = true) override;

	public:
		boost::signals2::connection ConnectCastling(const boost::signals2::signal<void(Coordinate, eCastleSide)>::slot_type& subscriber);
	};
}
