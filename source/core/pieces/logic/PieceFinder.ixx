module;
#include <memory>
#include <optional>
#include <unordered_map>
export module Chess.Core.PieceFinder;
import Chess.Core.Coordinate;
import Chess.Core.Piece;
import Chess.Core.PieceColorAndType;

namespace Chess::Core
{
    export class PieceFinder
    {
    public:
        explicit PieceFinder(std::unordered_map<Coordinate, std::shared_ptr<Piece>>&& pieceMap)
            : pieceMap_(std::move(pieceMap))
        {
        }

        std::shared_ptr<Piece> TryFind(const Coordinate& coordinate) const
        {
            const auto iter = pieceMap_.find(coordinate);
            if (iter == pieceMap_.end())
            {
                return nullptr;
            }
            return iter->second;
        }

        std::optional<Coordinate> TryFind(PieceColorAndType colorAndType) const
        {
            for (const auto& [coordinate, currentPiece] : pieceMap_)
            {
                if (colorAndType == currentPiece->GetColorAndType())
                {
                    return coordinate;
                }
            }
            return std::nullopt;
        }

    private:
        std::unordered_map<Coordinate, std::shared_ptr<Piece>> pieceMap_;
    };
} // namespace Chess::Core
