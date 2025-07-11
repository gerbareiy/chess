module;
#include <memory>
#include <stdexcept>
#include <vector>
export module Chess.KingChecker;
import Chess.Coordinate;
import Chess.Counts;
import Chess.eError;
import Chess.ErrorConverter;
import Chess.Piece;
import Chess.PieceFinder;
import Chess.ICastable;
import Chess.IMoveChecker;
import Chess.King;
import Chess.Sizes;

std::vector<Chess::Coordinate> FindCastlingMoves(const std::shared_ptr<Chess::King>& king, const std::shared_ptr<Chess::PieceFinder>& finder)
{
    std::vector<Chess::Coordinate> castlingMoves;

    if (!king->GetIsCheck() && king->GetCanMakeCastling())
    {
        const auto leftRook  = finder->Find({ .file = 'A', .rank = king->GetPosition().rank });
        const auto rightRook = finder->Find({ .file = 'A' + Chess::CHESSBOARD_SIZE - 1, .rank = king->GetPosition().rank });

        const auto canCastleLeft = leftRook && std::dynamic_pointer_cast<Chess::ICastable>(leftRook)
                                   && std::dynamic_pointer_cast<Chess::ICastable>(leftRook)->GetCanMakeCastling()
                                   && leftRook->GetColorAndType().color == king->GetColorAndType().color;

        const auto canCastleRight = rightRook && std::dynamic_pointer_cast<Chess::ICastable>(rightRook)
                                    && std::dynamic_pointer_cast<Chess::ICastable>(rightRook)->GetCanMakeCastling()
                                    && rightRook->GetColorAndType().color == king->GetColorAndType().color;

        if (canCastleLeft)
        {
            auto pathClear = true;
            for (char file = 'B'; file < king->GetPosition().file; ++file)
            {
                if (finder->Find({ .file = file, .rank = king->GetPosition().rank }))
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
            for (char file = king->GetPosition().file + 1; file < 'A' + Chess::CHESSBOARD_SIZE - 1; ++file)
            {
                if (finder->Find({ .file = file, .rank = king->GetPosition().rank }))
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

std::vector<Chess::Coordinate> FindPossibleMoves(const std::shared_ptr<Chess::King>& king, const std::vector<std::shared_ptr<Chess::Piece>>& piecesOnBoard)
{
    if (king->GetPosition().file < 'A' || king->GetPosition().file > 'A' + Chess::CHESSBOARD_SIZE - 1 || king->GetPosition().rank < 1
        || king->GetPosition().rank > Chess::CHESSBOARD_SIZE)
    {
        throw std::out_of_range(Chess::ErrorConverter::ToString(Chess::eError::OUT_OF_CHESSBOARD));
    }

    std::vector<Chess::Coordinate> moves;
    moves.reserve(Chess::COUNT_OF_KING_WAYS);

    const auto finder = std::make_shared<Chess::PieceFinder>(piecesOnBoard);

    for (auto deltaFile = -1; deltaFile <= 1; ++deltaFile)
    {
        for (auto deltaRank = -1; deltaRank <= 1; ++deltaRank)
        {
            if (deltaFile == 0 && deltaRank == 0)
            {
                continue;
            }

            char newFile = king->GetPosition().file + deltaFile;
            int  newRank = king->GetPosition().rank + deltaRank;

            const auto piece = finder->Find({ .file = newFile, .rank = newRank });

            if (newFile >= 'A' && newFile <= 'A' + Chess::CHESSBOARD_SIZE - 1 && newRank >= 1 && newRank <= Chess::CHESSBOARD_SIZE
                && (!piece || piece->GetColorAndType().color != king->GetColorAndType().color))
            {
                moves.emplace_back(newFile, newRank);
            }
        }
    }

    auto castlingMoves = FindCastlingMoves(king, finder);
    moves.insert(moves.end(), castlingMoves.begin(), castlingMoves.end());

    return moves;
}

namespace Chess
{
    export class KingChecker final : public IMoveChecker
    {
    public:
        virtual std::vector<Coordinate> GetMoves(const std::shared_ptr<Piece>& piece, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const override
        {
            if (const auto king = std::dynamic_pointer_cast<King>(piece))
            {
                return FindPossibleMoves(king, piecesOnBoard);
            }
            return {};
        }
    };
} // namespace Chess
