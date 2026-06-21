module;
#include <memory>
#include <optional>
#include <unordered_map>
export module Chess.PieceFinder;
import Chess.Coordinate;
import Chess.Piece;
import Chess.PieceColorAndType;

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

        std::shared_ptr<Piece> TryFind(const Coordinate& coordinate) const
        {
            const auto iter = m_pieceMap.find(coordinate);
            if (iter == m_pieceMap.end())
            {
                return nullptr;
            }
            return iter->second;
        }

        std::optional<Coordinate> TryFind(PieceColorAndType colorAndType) const
        {
            for (const auto& [coordinate, currentPiece] : m_pieceMap)
            {
                if (colorAndType == currentPiece->GetColorAndType())
                {
                    return coordinate;
                }
            }
            return std::nullopt;
        }
    };
} // namespace Chess
