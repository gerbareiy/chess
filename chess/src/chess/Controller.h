#pragma once

#include "Chessboard.h"
#include "pieces/logic/ePieceColor.h"
#include "logic/Coordinate.h"

#include <boost/signals2.hpp>

#include <memory>

namespace Chess
{
	class Controller
	{
	private:
		std::shared_ptr<Chessboard> m_chessboard;

	private:
		boost::signals2::signal<void()> m_signalMove;

	public:
		Controller(std::shared_ptr<Chessboard>& chessboard);

	public:
		bool TryInitPiece(Coordinate from) const;
		bool TryMovePiece(Coordinate to) const;

	public:
		boost::signals2::connection ConnectMove(const boost::signals2::signal<void()>::slot_type& subscriber);
	};
}
