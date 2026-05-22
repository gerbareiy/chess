module;
#include <expected>
#include <memory>
#include <stdexcept>
#include <vector>
export module Chess.KnightChecker;
import Chess.Coordinate;
import Chess.CoordinateToPieceBuilder;
import Chess.Counts;
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
        std::vector<std::pair<int, int>> m_knightMoveDirections = { { 2, 1 }, { 2, -1 }, { -2, 1 }, { -2, -1 }, { 1, 2 }, { 1, -2 }, { -1, 2 }, { -1, -2 } };

        std::expected<std::vector<Coordinate>, std::string> FindPossibleMoves(
            const std::shared_ptr<Piece>& knight, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const
        {
            if (knight == nullptr)
            {
                return std::unexpected("Piece is nullptr");
            }

            const auto position = knight->GetPosition();
            if (position.file < 'A' || position.file >= 'A' + CHESSBOARD_SIZE || position.rank < 1 || position.rank > CHESSBOARD_SIZE)
            {
                return std::unexpected("ChessPiece is out of the Chessboard");
            }

            std::vector<Coordinate> moves;
            moves.reserve(KNIGHT_WAYS_COUNT);

            auto       pieceMap = CoordinateToPieceBuilder::Build(piecesOnBoard);
            const auto finder   = std::make_shared<PieceFinder>(std::move(pieceMap));

            for (const auto& [fileDirection, rankDirection] : m_knightMoveDirections)
            {
                char newFile = position.file + fileDirection;
                int  newRank = position.rank + rankDirection;

                if (!PositionChecker::IsInChessboard({ .file = newFile, .rank = newRank }))
                {
                    continue;
                }

                const auto piece = finder->Find({ .file = newFile, .rank = newRank });
                if (!piece || piece->GetColorAndType().color != knight->GetColorAndType().color)
                {
                    moves.emplace_back(newFile, newRank);
                }
            }

            return moves;
        }

    public:
        virtual std::vector<Coordinate> GetMoves(const std::shared_ptr<Piece>& piece, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const override
        {
            auto possibleMoves = FindPossibleMoves(std::dynamic_pointer_cast<Knight>(piece), piecesOnBoard);
            if (possibleMoves.has_value())
            {
                return possibleMoves.value();
            }
            return {};
        }
    };
} // namespace Chess
