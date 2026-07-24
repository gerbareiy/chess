module;
#include <expected>
#include <memory>
#include <vector>
export module Chess.Core.KingChecker;
import Chess.Constants.Counts;
import Chess.Constants.Sizes;
import Chess.Core.Coordinate;
import Chess.Core.CoordinateToPieceFactory;
import Chess.Core.ePieceColor;
import Chess.Core.ICastable;
import Chess.Core.IMoveChecker;
import Chess.Core.King;
import Chess.Core.Piece;
import Chess.Core.PieceFinder;
import Chess.Utils.Exceptions;

namespace Chess::Core
{
    export class KingChecker final : public IMoveChecker
    {
    public:
        explicit KingChecker(const std::shared_ptr<King>& king)
            : king_(king)
        {
        }

        virtual std::vector<Coordinate> GetMoves(const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const override
        {
            if (king_ == nullptr)
            {
                throw Utils::PieceIsNullptrException();
            }
            return FindPossibleMoves(king_, king_->GetPosition(), king_->GetColorAndType().color, piecesOnBoard);
        }

    private:
        std::shared_ptr<King> king_;

        static std::vector<Coordinate> FindCastlingMoves(const std::shared_ptr<const King>& king, const std::shared_ptr<PieceFinder>& finder)
        {
            std::vector<Coordinate> castlingMoves;

            if (!king->GetIsCheck() && king->GetCanMakeCastling())
            {
                const auto leftRook  = finder->TryFind({ .file = 'A', .rank = king->GetPosition().rank });
                const auto rightRook = finder->TryFind({ .file = 'A' + Constants::Sizes::CHESSBOARD_SIZE - 1, .rank = king->GetPosition().rank });

                const auto canCastleLeft = leftRook && std::dynamic_pointer_cast<ICastable>(leftRook)
                                           && std::dynamic_pointer_cast<ICastable>(leftRook)->GetCanMakeCastling()
                                           && leftRook->GetColorAndType().color == king->GetColorAndType().color;

                const auto canCastleRight = rightRook && std::dynamic_pointer_cast<ICastable>(rightRook)
                                            && std::dynamic_pointer_cast<ICastable>(rightRook)->GetCanMakeCastling()
                                            && rightRook->GetColorAndType().color == king->GetColorAndType().color;

                if (canCastleLeft)
                {
                    auto pathClear = true;
                    for (char file = 'B'; file < king->GetPosition().file; ++file)
                    {
                        if (finder->TryFind({ .file = file, .rank = king->GetPosition().rank }))
                        {
                            pathClear = false;
                            break;
                        }
                    }

                    if (pathClear)
                    {
                        castlingMoves.emplace_back(king->GetPosition().file - 2, king->GetPosition().rank);
                    }
                }

                if (canCastleRight)
                {
                    auto pathClear = true;
                    for (char file = king->GetPosition().file + 1; file < 'A' + Constants::Sizes::CHESSBOARD_SIZE - 1; ++file)
                    {
                        if (finder->TryFind({ .file = file, .rank = king->GetPosition().rank }))
                        {
                            pathClear = false;
                            break;
                        }
                    }

                    if (pathClear)
                    {
                        castlingMoves.emplace_back(king->GetPosition().file + 2, king->GetPosition().rank);
                    }
                }
            }

            return castlingMoves;
        }

        static std::vector<Coordinate> FindPossibleMoves(
            const std::shared_ptr<const King>& king, Coordinate position, ePieceColor color, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard)
        {
            if (position.file < 'A' || position.file > 'A' + Constants::Sizes::CHESSBOARD_SIZE - 1 || position.rank < 1
                || position.rank > Constants::Sizes::CHESSBOARD_SIZE)
            {
                return {};
            }

            auto       pieceMap = CoordinateToPieceFactory::Create(piecesOnBoard);
            const auto finder   = std::make_shared<PieceFinder>(std::move(pieceMap));

            std::vector<Coordinate> result;
            result.reserve(Constants::Counts::KING_WAYS_COUNT);
            for (auto deltaFile = -1; deltaFile <= 1; ++deltaFile)
            {
                for (auto deltaRank = -1; deltaRank <= 1; ++deltaRank)
                {
                    if (deltaFile == 0 && deltaRank == 0)
                    {
                        continue;
                    }

                    char newFile = position.file + deltaFile;
                    int  newRank = position.rank + deltaRank;

                    const auto piece = finder->TryFind({ .file = newFile, .rank = newRank });

                    if (newFile >= 'A' && newFile <= 'A' + Constants::Sizes::CHESSBOARD_SIZE - 1 && newRank >= 1
                        && newRank <= Constants::Sizes::CHESSBOARD_SIZE && (!piece || piece->GetColorAndType().color != color))
                    {
                        result.emplace_back(newFile, newRank);
                    }
                }
            }

            auto castlingMoves = FindCastlingMoves(king, finder);
            result.insert_range(result.end(), std::move(castlingMoves));

            return result;
        }
    };
} // namespace Chess::Core
