#include "Inputer.h"

boost::signals2::connection Chess::Inputer::ConnectEnter(boost::signals2::signal<void(std::string)>::slot_type const& subscriber)
{
	return m_signalEnter.connect(subscriber);
}
