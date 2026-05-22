module;
#include <memory>
#include <unordered_map>
export module Chess.PieceFinder;
import Chess.Coordinate;
import Chess.Piece;

namespace Chess
{
    export class PieceFinder
    {
        std::unordered_map<Coordinate, std::shared_ptr<Piece>> m_pieceMap;

    public:
        explicit PieceFinder(std::unordered_map<Coordinate, std::shared_ptr<Piece>>&& pieceMap)
            : m_pieceMap(std::move(pieceMap))
        {
        }

        std::shared_ptr<Piece> Find(const Coordinate& coordinate) const
        {
            const auto it = m_pieceMap.find(coordinate);

            if (it == m_pieceMap.end())
            {
                return nullptr;
            }

            return it->second;
        }
    };
} // namespace Chess
