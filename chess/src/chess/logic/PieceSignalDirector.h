#pragma once

#include <boost/signals2.hpp>

namespace Chess
{
	class PieceSignalDirector
	{

	private:
		boost::signals2::signal<void()> m_moveSignal;
		boost::signals2::signal<void(bool)> m_moveWithCheckSignal;

	public:
		boost::signals2::connection ConnectMove(const boost::signals2::signal<void()>::slot_type& subscriber);
		boost::signals2::connection ConnectMoveWithCheck(const boost::signals2::signal<void(bool)>::slot_type& subscriber);
		void Invite();
		void Invite(bool isCheck);
	};
}
