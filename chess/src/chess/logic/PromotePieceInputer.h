#pragma once

#include "../pieces/logic/ePieceType.h"
#include "Inputer.h"

namespace Chess
{
    class PromotePieceInputer final : public Inputer
    {
    public:
        ePieceType Input() const;
    };
} // namespace Chess
