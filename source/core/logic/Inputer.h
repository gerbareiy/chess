#pragma once

#include <boost/signals2.hpp>

#include <string>

namespace Chess
{
    class Inputer
    {
    protected:
        boost::signals2::signal<void(std::string)> m_signalEnter;

    public:
        virtual ~Inputer() = default;

        virtual boost::signals2::connection ConnectEnter(const boost::signals2::signal<void(std::string)>::slot_type& subscriber);
    };
} // namespace Chess
