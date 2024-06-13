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

char Chess::Coordinate::get_File() const
{
	return m_file;
}

void Chess::Coordinate::set_File(char file)
{
	m_file = file;
}

int Chess::Coordinate::get_Rank() const
{
	return m_rank;
}

void Chess::Coordinate::set_Rank(int rank)
{
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
	if (get_File() == other.get_File())
	{
		return get_Rank() < other.get_Rank();
	}

	return get_File() < other.get_File();
}
