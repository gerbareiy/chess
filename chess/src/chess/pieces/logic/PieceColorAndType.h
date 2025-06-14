#pragma once

#include "ePieceColor.h"
#include "ePieceType.h"

namespace Chess
{
	struct PieceColorAndType
	{
		ePieceColor m_color;
		ePieceType m_type;

	public:
		PieceColorAndType();
		PieceColorAndType(ePieceColor color, ePieceType type);

	public:
		ePieceColor GetColor() const;
		ePieceType GetType() const;

		PieceColorAndType& operator=(PieceColorAndType const& other);
		bool operator==(PieceColorAndType const& other) const;
	};
}
