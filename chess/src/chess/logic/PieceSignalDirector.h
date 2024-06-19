#pragma once

#include <boost/signals2.hpp>

namespace Chess
{
	class PieceSignalDirector
	{

	private:
		boost::signals2::signal<void()> m_moveSignal{};

	public:
		boost::signals2::connection ConnectMove(const boost::signals2::signal<void()>::slot_type& subscriber);
		void Invite();
	};
}
