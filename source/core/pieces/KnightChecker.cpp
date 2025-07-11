#include "KnightChecker.h"

#include "Knight.h"
#include "Piece.h"
#include "core/logic/Coordinate.h"
#include "core/logic/Counts.h"
#include "core/logic/ErrorConverter.h"
#include "core/logic/Sizes.h"
#include "core/logic/eError.h"
#include "logic/PieceColorAndType.h"
#include "logic/PieceFinder.h"
#include "logic/ePieceType.h"

#include <memory>
#include <stdexcept>

std::vector<Chess::Coordinate> Chess::KnightChecker::FindPossibleMoves(const std::shared_ptr<Piece>&             knight,
                                                                       const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const
{
    if (knight->GetPosition().file < 'A' || knight->GetPosition().file >= 'A' + CHESSBOARD_SIZE || knight->GetPosition().rank < 1
        || knight->GetPosition().rank > CHESSBOARD_SIZE)
    {
        throw std::out_of_range(ErrorConverter::ToString(eError::OUT_OF_CHESSBOARD));
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

        if (!piece || piece->GetColorAndType().GetColor() != knight->GetColorAndType().GetColor())
        {
            moves.emplace_back(newFile, newRank);
        }
    }

    return moves;
}

std::vector<Chess::Coordinate> Chess::KnightChecker::GetMoves(const std::shared_ptr<Piece>&              piece,
                                                              const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const
{
    if (!piece || typeid(*piece) != typeid(Knight) || piece->GetColorAndType().GetType() != ePieceType::KNIGHT)
    {
        return {};
    }

    return FindPossibleMoves(std::static_pointer_cast<Knight>(piece), piecesOnBoard);
}
