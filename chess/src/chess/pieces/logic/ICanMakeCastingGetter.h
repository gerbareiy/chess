#pragma once

class ICanMakeCastlingGetter
{
public:
	virtual ~ICanMakeCastlingGetter() = default;

public:
	virtual bool get_CanMakeCastling() const = 0;
};