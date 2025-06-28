#pragma once

#include "eError.h"

#include <string>

namespace Chess
{
    class ErrorConverter
    {
    public:
        static std::string ToString(eError error);
    };
} // namespace Chess