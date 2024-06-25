#pragma once

#include "Chessboard.h"
#include "logic/eConsoleColor.h"
#include "pieces/logic/ePieceColor.h"

#include <boost/signals2.hpp>

#include <memory>
#include <string>
#include <windows.h>

namespace Chess
{
	class InputHandler
	{
	private:
		boost::signals2::signal<void(std::string)> m_signalEnter;

	private:
		Coordinate EnterCoordinate() const;
		char EnterFile() const;
		int EnterRank() const;

	public:
		Coordinate EnterFrom() const;
		Coordinate EnterTo() const;

	public:
		boost::signals2::connection ConnectEnter(const boost::signals2::signal<void(std::string)>::slot_type& subscriber);
	};
}
