#include "MoveChecker.h"

#include "../../logic/Coordinate.h"
#include "../Piece.h"
#include "CheckChecker.h"
#include "MoveCheckerFactory.h"
#include "PieceFinder.h"

#include "stdexcept"
#include <vector>

std::vector<Chess::Coordinate> Chess::MoveChecker::FindUncheckedMove(const Coordinate& move, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const
{
    constexpr auto          checker = CheckChecker();
    std::vector<Coordinate> filteredMoves;
    const auto              finder        = std::make_shared<PieceFinder>(piecesOnBoard);
    const auto              capturedPiece = finder->Find(move);

    std::vector<std::shared_ptr<Chess::Piece>> tempPiecesOnBoard = piecesOnBoard;

    if (capturedPiece)
    {
        std::erase(tempPiecesOnBoard, capturedPiece);
    }

    m_piece->Move(move, false);

    if (!checker.IsCheck(m_piece->GetColorAndType().GetColor(), tempPiecesOnBoard))
    {
        filteredMoves.emplace_back(move);
    }

    return filteredMoves;
}

Chess::MoveChecker::MoveChecker(const std::shared_ptr<Piece>& piece)
    : m_moveCheckerOfPiece(MoveCheckerFactory::Create(piece))
    , m_piece(piece)
{
}

std::vector<Chess::Coordinate> Chess::MoveChecker::GetFilteredMoves(const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const
{
    std::vector<Coordinate> filteredMoves;
    const auto              notFilteredMoves = m_moveCheckerOfPiece->GetMoves(m_piece, piecesOnBoard);
    const auto              pieceCoordinate  = m_piece->GetPosition();

    for (const auto& move : notFilteredMoves)
    {
        auto partOfFilteredMoves = FindUncheckedMove(move, piecesOnBoard);
        filteredMoves.insert(filteredMoves.end(), partOfFilteredMoves.begin(), partOfFilteredMoves.end());
    }

    m_piece->Move(pieceCoordinate, false);

    return filteredMoves;
}
