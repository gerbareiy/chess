#pragma once

#include "IMoveChecker.h"

#include <memory>

namespace Chess
{
    class Piece;

    class MoveCheckerFactory
    {
    public:
        static std::shared_ptr<IMoveChecker> Create(const std::shared_ptr<Piece>& piece);
    };
} // namespace Chess
