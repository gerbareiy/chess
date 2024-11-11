#pragma once

#include "logic/eConsoleColor.h"
#include "logic/Inputer.h"
#include "pieces/logic/ePieceColor.h"

#include <boost/signals2.hpp>

#include <memory>
#include <string>
#include <windows.h>

namespace Chess
{
	struct Coordinate;

	class HandlerInputer : public Inputer
	{
	private:
		Coordinate EnterCoordinate() const;
		char EnterFile() const;
		int EnterRank() const;

	public:
		Coordinate EnterFrom() const;
		Coordinate EnterTo() const;
	};
}
