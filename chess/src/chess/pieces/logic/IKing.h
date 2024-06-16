#pragma once

class IKing
{
public:
	virtual ~IKing() = default;

public:
	virtual bool get_CanMakeCastling() const = 0;
	virtual bool get_IsCheck() const = 0;
};