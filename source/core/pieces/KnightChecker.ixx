module;
#include <expected>
#include <memory>
#include <stdexcept>
#include <vector>
export module Chess.KnightChecker;
import Chess.Coordinate;
import Chess.Counts;
import Chess.eError;
import Chess.ePieceType;
import Chess.ErrorConverter;
import Chess.IMoveChecker;
import Chess.Knight;
import Chess.Piece;
import Chess.PieceFinder;
import Chess.Sizes;

namespace Chess
{
    export class KnightChecker final : public IMoveChecker
    {
        std::vector<std::pair<int, int>> m_knightMoveDirections = { { 2, 1 }, { 2, -1 }, { -2, 1 }, { -2, -1 }, { 1, 2 }, { 1, -2 }, { -1, 2 }, { -1, -2 } };

        std::expected<std::vector<Coordinate>, std::string> FindPossibleMoves(const std::shared_ptr<Piece>&              knight,
                                                                              const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const
        {
            const auto knightPosition = knight->GetPosition();
            if (knightPosition.file < 'A' || knightPosition.file >= 'A' + CHESSBOARD_SIZE || knightPosition.rank < 1 || knightPosition.rank > CHESSBOARD_SIZE)
            {
                return std::unexpected(ErrorConverter::ToString(eError::OUT_OF_CHESSBOARD));
            }

            std::vector<Coordinate> moves;
            moves.reserve(COUNT_OF_KNIGHT_WAYS);

            const auto currentPos = knight->GetPosition();
            const auto finder     = std::make_shared<PieceFinder>(piecesOnBoard);

            for (const auto& [first, second] : m_knightMoveDirections)
            {
                char newFile = currentPos.file + first;
                int  newRank = currentPos.rank + second;

                if (newFile < 'A' || newFile >= 'A' + CHESSBOARD_SIZE || newRank < 1 || newRank > CHESSBOARD_SIZE)
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
            if (!piece || typeid(*piece) != typeid(Knight) || piece->GetColorAndType().type != ePieceType::KNIGHT)
            {
                return {};
            }

            auto possibleMoves = FindPossibleMoves(std::static_pointer_cast<Knight>(piece), piecesOnBoard);
            if (possibleMoves)
            {
                return *possibleMoves;
            }
            else
            {
                return {}; // TODO: somehow call checkmate here
            }
        }
    };
} // namespace Chess
