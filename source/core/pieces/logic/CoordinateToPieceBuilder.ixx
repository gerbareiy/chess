module;
#include <memory>
#include <unordered_map>
#include <vector>
export module Chess.CoordinateToPieceBuilder;
import Chess.Coordinate;
import Chess.Piece;

namespace Chess
{
    export class CoordinateToPieceBuilder
    {
    public:
        static std::unordered_map<Coordinate, std::shared_ptr<Piece>> Build(const std::vector<std::shared_ptr<Piece>>& pieces)
        {
            std::unordered_map<Coordinate, std::shared_ptr<Piece>> result;
            result.reserve(pieces.size());
            for (const auto& piece : pieces)
            {
                result[piece->GetPosition()] = piece;
            }
            return result;
        }
    };
} // namespace Chess
