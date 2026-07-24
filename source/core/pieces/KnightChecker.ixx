module;
#include <array>
#include <expected>
#include <memory>
#include <vector>
export module Chess.Core.KnightChecker;
import Chess.Constants.Counts;
import Chess.Constants.Sizes;
import Chess.Core.Coordinate;
import Chess.Core.CoordinateToPieceFactory;
import Chess.Core.ePieceColor;
import Chess.Core.ePieceType;
import Chess.Core.IMoveChecker;
import Chess.Core.Knight;
import Chess.Core.Piece;
import Chess.Core.PieceFinder;
import Chess.Core.PositionChecker;
import Chess.Utils.Exceptions;

namespace Chess::Core
{
    export class KnightChecker final : public IMoveChecker
    {
    public:
        explicit KnightChecker(const std::shared_ptr<Knight>& knight)
            : knight_(knight)
        {
        }

        virtual std::vector<Coordinate> GetMoves(const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const override
        {
            if (knight_ == nullptr)
            {
                throw Utils::PieceIsNullptrException();
            }
            return FindPossibleMoves(knight_->GetPosition(), knight_->GetColorAndType().color, piecesOnBoard);
        }

    private:
        std::shared_ptr<Knight> knight_;

        static std::vector<Coordinate> FindPossibleMoves(
            Coordinate position, ePieceColor color, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard)
        {
            if (position.file < 'A' || position.file >= 'A' + Constants::Sizes::CHESSBOARD_SIZE || position.rank < 1
                || position.rank > Constants::Sizes::CHESSBOARD_SIZE)
            {
                return {};
            }

            auto       pieceMap = CoordinateToPieceFactory::Create(piecesOnBoard);
            const auto finder   = std::make_shared<PieceFinder>(std::move(pieceMap));

            std::array constexpr knightMoveDirections = { std::pair(2, 1), std::pair(2, -1), std::pair(-2, 1), std::pair(-2, -1),
                                                          std::pair(1, 2), std::pair(1, -2), std::pair(-1, 2), std::pair(-1, -2) };

            std::vector<Coordinate> result;
            result.reserve(Constants::Counts::KNIGHT_WAYS_COUNT);
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
    };
} // namespace Chess::Core
