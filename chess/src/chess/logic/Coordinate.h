#pragma once

namespace Chess
{
	struct Coordinate
	{
	private:
		char m_file;
		int m_rank;

	public:
		Coordinate();
		Coordinate(char file, int rank);

	public:
		char get_File() const;
		void set_File(char file);
		int get_Rank() const;
		void set_Rank(int rank);

	public:
		Coordinate& operator=(const Coordinate& other);
		bool operator==(const Coordinate& other) const;
		bool operator<(const Coordinate& other) const;
	};
}
