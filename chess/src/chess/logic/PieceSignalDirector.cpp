#include "PieceSignalDirector.h"

boost::signals2::connection Chess::PieceSignalDirector::ConnectMove(boost::signals2::signal<void()>::slot_type const& subscriber)
{
	return m_moveSignal.connect(subscriber);
}

boost::signals2::connection Chess::PieceSignalDirector::ConnectMoveWithCheck(boost::signals2::signal<void(bool)>::slot_type const& subscriber)
{
	return m_moveWithCheckSignal.connect(subscriber);
}

void Chess::PieceSignalDirector::Invite() const
{
	m_moveSignal();
}

void Chess::PieceSignalDirector::Invite(bool isCheck) const
{
	m_moveWithCheckSignal(isCheck);
}
