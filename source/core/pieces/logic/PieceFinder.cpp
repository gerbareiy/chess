#include "PieceFinder.h"

#include "core/pieces/Piece.h"

Chess::PieceFinder::PieceFinder(const std::vector<std::shared_ptr<Piece>>& pieces)
{
    for (const auto& piece : pieces)
    {
        m_pieceMap[piece->GetPosition()] = piece;
    }
}

std::shared_ptr<Chess::Piece> Chess::PieceFinder::Find(const Coordinate& coordinate) const
{
    const auto it = m_pieceMap.find(coordinate);

    if (it == m_pieceMap.end())
    {
        return nullptr;
    }

    return it->second;
}
