#pragma once

namespace Chess
{
	class  ICastable
	{
	public:
		virtual ~ICastable() = default;

	public:
		virtual bool get_CanMakeCastling() const = 0;
	};
}
