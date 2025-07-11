#include "KingChecker.h"

#include "King.h"
#include "Piece.h"
#include "core/logic/Coordinate.h"
#include "core/logic/Counts.h"
#include "core/logic/ErrorConverter.h"
#include "core/logic/Sizes.h"
#include "core/logic/eError.h"
#include "logic/ICastlable.h"
#include "logic/PieceColorAndType.h"
#include "logic/PieceFinder.h"
#include "logic/ePieceType.h"

#include <stdexcept>

static std::vector<Chess::Coordinate> FindCastlingMoves(const std::shared_ptr<Chess::King>& king, const std::shared_ptr<Chess::PieceFinder>& finder)
{
    std::vector<Chess::Coordinate> castlingMoves;

    if (!king->GetIsCheck() && king->GetCanMakeCastling())
    {
        const auto leftRook  = finder->Find(Chess::Coordinate('A', king->GetPosition().GetRank()));
        const auto rightRook = finder->Find(Chess::Coordinate('A' + Chess::CHESSBOARD_SIZE - 1, king->GetPosition().GetRank()));

        const auto canCastleLeft = leftRook && std::dynamic_pointer_cast<Chess::ICastable>(leftRook)
                                   && std::dynamic_pointer_cast<Chess::ICastable>(leftRook)->GetCanMakeCastling()
                                   && leftRook->GetColorAndType().GetColor() == king->GetColorAndType().GetColor();

        const auto canCastleRight = rightRook && std::dynamic_pointer_cast<Chess::ICastable>(rightRook)
                                    && std::dynamic_pointer_cast<Chess::ICastable>(rightRook)->GetCanMakeCastling()
                                    && rightRook->GetColorAndType().GetColor() == king->GetColorAndType().GetColor();

        if (canCastleLeft)
        {
            auto pathClear = true;
            for (char file = 'B'; file < king->GetPosition().GetFile(); ++file)
            {
                if (finder->Find(Chess::Coordinate(file, king->GetPosition().GetRank())))
                {
                    pathClear = false;
                    break;
                }
            }

            if (pathClear)
            {
                castlingMoves.emplace_back(king->GetPosition().GetFile() - 2, king->GetPosition().GetRank());
            }
        }

        if (canCastleRight)
        {
            auto pathClear = true;
            for (char file = king->GetPosition().GetFile() + 1; file < 'A' + Chess::CHESSBOARD_SIZE - 1; ++file)
            {
                if (finder->Find(Chess::Coordinate(file, king->GetPosition().GetRank())))
                {
                    pathClear = false;
                    break;
                }
            }

            if (pathClear)
            {
                castlingMoves.emplace_back(king->GetPosition().GetFile() + 2, king->GetPosition().GetRank());
            }
        }
    }

    return castlingMoves;
}

static std::vector<Chess::Coordinate> FindPossibleMoves(const std::shared_ptr<Chess::King>&               king,
                                                        const std::vector<std::shared_ptr<Chess::Piece>>& piecesOnBoard)
{
    if (king->GetPosition().GetFile() < 'A' || king->GetPosition().GetFile() > 'A' + Chess::CHESSBOARD_SIZE - 1 || king->GetPosition().GetRank() < 1
        || king->GetPosition().GetRank() > Chess::CHESSBOARD_SIZE)
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

            auto newFile = king->GetPosition().GetFile() + deltaFile;
            auto newRank = king->GetPosition().GetRank() + deltaRank;

            const auto piece = finder->Find(Chess::Coordinate(newFile, newRank));

            if (newFile >= 'A' && newFile <= 'A' + Chess::CHESSBOARD_SIZE - 1 && newRank >= 1 && newRank <= Chess::CHESSBOARD_SIZE
                && (!piece || piece->GetColorAndType().GetColor() != king->GetColorAndType().GetColor()))
            {
                moves.emplace_back(newFile, newRank);
            }
        }
    }

    auto castlingMoves = FindCastlingMoves(king, finder);
    moves.insert(moves.end(), castlingMoves.begin(), castlingMoves.end());

    return moves;
}

std::vector<Chess::Coordinate> Chess::KingChecker::GetMoves(const std::shared_ptr<Piece>& piece, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const
{
    if (!piece || typeid(*piece) != typeid(King) || piece->GetColorAndType().GetType() != ePieceType::KING)
    {
        return {};
    }

    return FindPossibleMoves(std::static_pointer_cast<King>(piece), piecesOnBoard);
}
