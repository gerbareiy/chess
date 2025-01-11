#include "PieceFinder.h"

#include "../Piece.h"
#include "../../logic/Coordinate.h"

Chess::PieceFinder::PieceFinder(const std::vector<std::shared_ptr<Piece>>& pieces)
{
	for (const auto& piece : pieces)
	{
		m_pieceMap[piece->GetPosition()] = piece;
	}
}

std::shared_ptr<Chess::Piece> Chess::PieceFinder::Find(Coordinate coordinate) const
{
	auto it = m_pieceMap.find(coordinate);

	if (it == m_pieceMap.end())
	{
		return nullptr;
	}

	return it->second;
}
