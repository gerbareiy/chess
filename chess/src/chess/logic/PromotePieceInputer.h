#pragma once

#include "Inputer.h"
#include "../pieces/logic/ePieceType.h"

namespace Chess
{
	class PromotePieceInputer : public Inputer
	{
	public:
		ePieceType Input() const;
	};
}
