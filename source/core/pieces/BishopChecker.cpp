#include "BishopChecker.h"

#include "Bishop.h"
#include "core/logic/Coordinate.h"
#include "core/logic/Counts.h"
#include "logic/BishopQueenRookDirectionChecker.h"
#include "logic/PieceColorAndType.h"
#include "logic/PieceFinder.h"
#include "logic/ePieceType.h"

#include <memory>

std::vector<Chess::Coordinate> Chess::BishopChecker::FindPossibleMoves(const std::shared_ptr<Bishop>&             bishop,
                                                                       const std::vector<std::shared_ptr<Piece>>& piecesOnBoard)
{
    std::vector<Coordinate> moves;
    moves.reserve(COUNT_OF_BISHOP_WAYS);

    const auto finder = std::make_shared<PieceFinder>(piecesOnBoard);

    auto first  = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, bishop, { -1, -1 });
    auto second = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, bishop, { -1, 1 });
    auto third  = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, bishop, { 1, -1 });
    auto fourth = BishopQueenRookDirectionChecker::FindPossibleMoves(finder, bishop, { 1, 1 });

    moves.insert(moves.end(), first.begin(), first.end());
    moves.insert(moves.end(), second.begin(), second.end());
    moves.insert(moves.end(), third.begin(), third.end());
    moves.insert(moves.end(), fourth.begin(), fourth.end());

    return moves;
}

std::vector<Chess::Coordinate> Chess::BishopChecker::GetMoves(const std::shared_ptr<Piece>&              piece,
                                                              const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const
{
    if (!piece || typeid(*piece) != typeid(Bishop) || piece->GetColorAndType().GetType() != ePieceType::BISHOP)
    {
        return {};
    }

    return FindPossibleMoves(std::static_pointer_cast<Bishop>(piece), piecesOnBoard);
}
