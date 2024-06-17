#pragma once

#include "eCastleSide.h"
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
		virtual void OnCastling(Coordinate to, eCastleSide side) = 0;
	};
}
