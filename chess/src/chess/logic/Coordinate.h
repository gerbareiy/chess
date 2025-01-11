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
		char GetFile() const;
		void SetFile(char file);
		int GetRank() const;
		void SetRank(int rank);

		Coordinate& operator=(const Coordinate& other);
		bool operator==(const Coordinate& other) const;
		bool operator<(const Coordinate& other) const;
	};
}
