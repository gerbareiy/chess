#include "MoveChecker.h"

#include "CheckChecker.h"
#include "MoveCheckerFactory.h"
#include "PieceColorAndType.h"
#include "PieceFinder.h"
#include "../Piece.h"
#include "../../logic/Coordinate.h"

#include "stdexcept"
#include <vector>

Chess::MoveChecker::MoveChecker(std::shared_ptr<Piece> const& piece)
    : m_piece(piece), m_moveCheckerOfPiece(std::unique_ptr<MoveCheckerFactory>()->Create(piece)) { }

std::vector<Chess::Coordinate> Chess::MoveChecker::FindUncheckedMove(Chess::Coordinate const& move, std::vector<std::shared_ptr<Chess::Piece>> const& piecesOnBoard) const
{
	auto const checker = std::unique_ptr<CheckChecker>();
    std::vector<Chess::Coordinate> filteredMoves;
    auto finder = std::make_shared<PieceFinder>(piecesOnBoard);
    auto capturedPiece = finder->Find(move);

    std::vector<std::shared_ptr<Chess::Piece>> tempPiecesOnBoard = piecesOnBoard;

    if (capturedPiece)
    {
        tempPiecesOnBoard.erase(std::remove(tempPiecesOnBoard.begin(), tempPiecesOnBoard.end(), capturedPiece), tempPiecesOnBoard.end());
    }

    m_piece->Move(move, false);

    if (!checker->IsCheck(m_piece->GetColorAndType().GetColor(), tempPiecesOnBoard))
    {
        filteredMoves.emplace_back(move);
    }

    return filteredMoves;
}

std::vector<Chess::Coordinate> Chess::MoveChecker::GetFilteredMoves(std::vector<std::shared_ptr<Piece>> const& piecesOnBoard) const
{
    std::vector<Chess::Coordinate> filteredMoves;
    auto notFilteredMoves = m_moveCheckerOfPiece->GetMoves(m_piece, piecesOnBoard);
    auto pieceCoordinate = m_piece->GetPosition();

    for (auto const& move : notFilteredMoves)
    {
        auto partOfFilteredMoves = FindUncheckedMove(move, piecesOnBoard);
        filteredMoves.insert(filteredMoves.end(), partOfFilteredMoves.begin(), partOfFilteredMoves.end());
    }

    m_piece->Move(pieceCoordinate, false);

    return filteredMoves;
}
