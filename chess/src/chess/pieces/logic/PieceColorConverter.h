#pragma once

#include "ePieceColor.h"

#include <string>

namespace Chess
{
	class PieceColorConverter
	{
	public:
		static std::string ConvertToString(ePieceColor color);
	};
}