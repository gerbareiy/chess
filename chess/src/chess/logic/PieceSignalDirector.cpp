#include "PieceSignalDirector.h"

boost::signals2::connection Chess::PieceSignalDirector::ConnectMove(const boost::signals2::signal<void()>::slot_type& subscriber)
{
	return m_moveSignal.connect(subscriber);
}

void Chess::PieceSignalDirector::Invite()
{
	m_moveSignal();
}
