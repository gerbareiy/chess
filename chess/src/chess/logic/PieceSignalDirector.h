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
		boost::signals2::connection ConnectMove(boost::signals2::signal<void()>::slot_type const& subscriber);
		boost::signals2::connection ConnectMoveWithCheck(boost::signals2::signal<void(bool)>::slot_type const& subscriber);
		void Invite() const;
		void Invite(bool isCheck) const;
	};
}
