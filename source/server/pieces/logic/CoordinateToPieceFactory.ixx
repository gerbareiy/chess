module;
#include <memory>
#include <unordered_map>
#include <vector>
export module Chess.CoordinateToPieceFactory;
import Chess.Coordinate;
import Chess.Piece;

namespace Chess
{
    export class CoordinateToPieceFactory
    {
    public:
        static std::unordered_map<Coordinate, std::shared_ptr<Piece>> Create(const std::vector<std::shared_ptr<Piece>>& pieces)
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
