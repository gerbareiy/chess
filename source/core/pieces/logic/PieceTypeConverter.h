#pragma once

#include "ePieceType.h"

#include <string>

namespace Chess
{
    class PieceTypeConverter
    {
    public:
        static std::string ConvertToString(ePieceType type);
    };
} // namespace Chess
