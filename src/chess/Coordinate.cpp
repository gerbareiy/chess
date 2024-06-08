#include "Coordinate.h"

Chess::Coordinate::Coordinate()
{
	m_file = 'A';
	m_rank = 1;
}

Chess::Coordinate::Coordinate(char file, int rank)
{
	m_file = file;
	m_rank = rank;
}

Chess::Coordinate& Chess::Coordinate::operator=(const Coordinate& other)
{
	if (this == &other)
	{
		return *this;
	}

	m_file = other.get_File();
	m_rank = other.get_Rank();

	return *this;
}

bool Chess::Coordinate::operator==(const Coordinate& other) const
{
	return get_File() == other.get_File() && get_Rank() == other.get_Rank();
}

bool Chess::Coordinate::operator<(const Coordinate& other) const
{
	if (this->get_File() == other.get_File())
	{
		return this->get_Rank() < other.get_Rank();
	}

	return this->get_File() < other.get_File();
}
