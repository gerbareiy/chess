#include "PieceFinder.h"

Chess::PieceFinder::PieceFinder(const std::vector<std::shared_ptr<IPiece>>& pieces)
{
	for (const auto& piece : pieces)
	{
		m_pieceMap[piece->get_Position()] = piece;
	}
}

std::shared_ptr<Chess::IPiece> Chess::PieceFinder::Find(Coordinate coordinate)
{
	auto it = m_pieceMap.find(coordinate);
	if (it == m_pieceMap.end())
	{
		return nullptr;
	}

	return it->second;
}
