#pragma once

#include <boost/signals2.hpp>

namespace Chess
{
	class Inputer
	{
	protected:
		boost::signals2::signal<void(std::string)> m_signalEnter;

	public:
		virtual ~Inputer() = default;

	public:
		virtual boost::signals2::connection ConnectEnter(boost::signals2::signal<void(std::string)>::slot_type const& subscriber);
	};
}