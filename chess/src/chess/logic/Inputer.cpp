#include "Inputer.h"

boost::signals2::connection Chess::Inputer::ConnectEnter(const boost::signals2::signal<void(std::string)>::slot_type& subscriber)
{
    return m_signalEnter.connect(subscriber);
}
