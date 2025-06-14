#pragma once

#include <boost/signals2.hpp>

#include <memory>

namespace Chess
{
	class Chessboard;
	struct Coordinate;

	class Controller
	{
	private:
		std::shared_ptr<Chessboard> m_chessboard;

	private:
		boost::signals2::signal<void()> m_signalMove;

	public:
		Controller(std::shared_ptr<Chessboard>& chessboard);

	public:
		bool TryInitPiece(Coordinate const& from) const;
		bool TryMovePiece(Coordinate const& to) const;

	public:
		boost::signals2::connection ConnectMove(boost::signals2::signal<void()>::slot_type const& subscriber);
	};
}
