#include "PieceColorAndType.h"

Chess::PieceColorAndType::PieceColorAndType()
{
	m_type = ePieceType::NONE;
	m_color = ePieceColor::NONE;
}

Chess::PieceColorAndType::PieceColorAndType(ePieceColor color, ePieceType type)
{
	m_color = color;
	m_type = type;
}

Chess::ePieceColor Chess::PieceColorAndType::GetColor() const
{
	return m_color;
}

Chess::ePieceType Chess::PieceColorAndType::GetType() const
{
	return m_type;
}

Chess::PieceColorAndType& Chess::PieceColorAndType::operator=(const PieceColorAndType& other)
{
	if (this == &other)
	{
		return *this;
	}

	m_color = other.GetColor();
	m_type = other.GetType();

	return *this;
}

bool Chess::PieceColorAndType::operator==(const PieceColorAndType& other) const
{
	return false;
}
