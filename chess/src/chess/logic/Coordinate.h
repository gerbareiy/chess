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

		Coordinate& operator=(Coordinate const& other);
		bool operator==(Coordinate const& other) const;
		bool operator<(Coordinate const& other) const;
	};
}
