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

char Chess::Coordinate::GetFile() const
{
	return m_file;
}

void Chess::Coordinate::SetFile(char file)
{
	m_file = file;
}

int Chess::Coordinate::GetRank() const
{
	return m_rank;
}

void Chess::Coordinate::SetRank(int rank)
{
	m_rank = rank;
}

Chess::Coordinate& Chess::Coordinate::operator=(Coordinate const& other)
{
	if (this == &other)
	{
		return *this;
	}

	m_file = other.GetFile();
	m_rank = other.GetRank();

	return *this;
}

bool Chess::Coordinate::operator==(Coordinate const& other) const
{
	return GetFile() == other.GetFile() && GetRank() == other.GetRank();
}

bool Chess::Coordinate::operator<(Coordinate const& other) const
{
	if (GetFile() == other.GetFile())
	{
		return GetRank() < other.GetRank();
	}

	return GetFile() < other.GetFile();
}
