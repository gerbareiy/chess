module;
#include <array>
#include <expected>
#include <memory>
#include <stdexcept>
#include <vector>
export module Chess.KnightChecker;
import Chess.Coordinate;
import Chess.CoordinateToPieceFactory;
import Chess.Counts;
import Chess.ePieceColor;
import Chess.ePieceType;
import Chess.IMoveChecker;
import Chess.Knight;
import Chess.Piece;
import Chess.PieceFinder;
import Chess.PositionChecker;
import Chess.Sizes;

namespace Chess
{
    export class KnightChecker final : public IMoveChecker
    {
        std::shared_ptr<Knight> m_knight;

        static std::vector<Coordinate> FindPossibleMoves(
            Coordinate position, ePieceColor color, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard)
        {
            if (position.file < 'A' || position.file >= 'A' + CHESSBOARD_SIZE || position.rank < 1 || position.rank > CHESSBOARD_SIZE)
            {
                return {};
            }

            auto       pieceMap = CoordinateToPieceFactory::Create(piecesOnBoard);
            const auto finder   = std::make_shared<PieceFinder>(std::move(pieceMap));

            std::array constexpr knightMoveDirections = { std::pair(2, 1), std::pair(2, -1), std::pair(-2, 1), std::pair(-2, -1),
                                                          std::pair(1, 2), std::pair(1, -2), std::pair(-1, 2), std::pair(-1, -2) };

            std::vector<Coordinate> result;
            result.reserve(KNIGHT_WAYS_COUNT);
            for (const auto& [fileDirection, rankDirection] : knightMoveDirections)
            {
                char newFile = position.file + fileDirection;
                int  newRank = position.rank + rankDirection;

                if (!PositionChecker::IsInChessboard({ .file = newFile, .rank = newRank }))
                {
                    continue;
                }

                const auto piece = finder->TryFind({ .file = newFile, .rank = newRank });
                if (!piece || piece->GetColorAndType().color != color)
                {
                    result.emplace_back(newFile, newRank);
                }
            }

            return result;
        }

    public:
        explicit KnightChecker(const std::shared_ptr<Knight>& knight)
            : m_knight(knight)
        {
        }

        virtual std::vector<Coordinate> GetMoves(const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const override
        {
            if (m_knight == nullptr)
            {
                throw std::logic_error("piece is nullptr");
            }
            return FindPossibleMoves(m_knight->GetPosition(), m_knight->GetColorAndType().color, piecesOnBoard);
        }
    };
} // namespace Chess
