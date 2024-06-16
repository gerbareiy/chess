#pragma once

#include "ICanMakeCastingGetter.h"
#include "../King.h"

namespace Chess
{
	class  ICastable : public ICanMakeCastlingGetter
	{
	public:
		virtual ~ICastable() = default;

	public:
		virtual bool get_CanMakeCastling() const = 0;

	public:
		virtual void OnCasting(King king) = 0;
	};
}
