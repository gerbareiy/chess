#include "KnightChecker.h"

#include "../logic/Coordinate.h"
#include "../logic/Counts.h"
#include "../logic/ErrorConverter.h"
#include "../logic/Sizes.h"
#include "../logic/eError.h"
#include "Knight.h"
#include "Piece.h"
#include "logic/PieceColorAndType.h"
#include "logic/PieceFinder.h"
#include "logic/ePieceType.h"

#include <memory>
#include <stdexcept>

std::vector<Chess::Coordinate> Chess::KnightChecker::FindPossibleMoves(const std::shared_ptr<Knight>&             knight,
                                                                       const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const
{
    if (knight->GetPosition().GetFile() < 'A' || knight->GetPosition().GetFile() >= 'A' + CHESSBOARD_SIZE || knight->GetPosition().GetRank() < 1
        || knight->GetPosition().GetRank() > CHESSBOARD_SIZE)
    {
        throw std::out_of_range(ErrorConverter::ToString(eError::OUT_OF_CHESSBOARD));
    }

    std::vector<Coordinate> moves;
    moves.reserve(COUNT_OF_KNIGHT_WAYS);

    const auto currentPos = knight->GetPosition();
    const auto finder     = std::make_shared<PieceFinder>(piecesOnBoard);

    for (const auto& move : m_knightMoveDirections)
    {
        auto newFile = currentPos.GetFile() + move.first;
        auto newRank = currentPos.GetRank() + move.second;

        if (newFile < 'A' || newFile >= 'A' + CHESSBOARD_SIZE || newRank < 1 || newRank > CHESSBOARD_SIZE)
        {
            continue;
        }

        const auto piece = finder->Find(Coordinate(newFile, newRank));

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
