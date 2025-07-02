#pragma once

#include "Inputer.h"
#include "core/pieces/logic/ePieceType.h"

namespace Chess
{
    class PromotePieceInputer final : public Inputer
    {
    public:
        ePieceType Input() const;
    };
} // namespace Chess
