#include "MoveChecker.h"

#include "CheckChecker.h"
#include "MoveCheckerFactory.h"
#include "PieceFinder.h"

#include "stdexcept"
#include <vector>

Chess::MoveChecker::MoveChecker(const std::shared_ptr<IPiece>& piece)
    : m_piece(piece), m_moveCheckerOfPiece(std::unique_ptr<MoveCheckerFactory>()->Create(piece)) { }

std::vector<Chess::Coordinate> Chess::MoveChecker::EmulateCheckIfMove(const Chess::Coordinate& move, const std::vector<std::shared_ptr<Chess::IPiece>>& piecesOnBoard)
{
    const auto checker = std::unique_ptr<CheckChecker>();
    std::vector<Chess::Coordinate> filteredMoves;
    auto finder = std::make_shared<PieceFinder>(piecesOnBoard);
    auto capturedPiece = finder->Find(move);

    std::vector<std::shared_ptr<Chess::IPiece>> tempPiecesOnBoard = piecesOnBoard;

    if (capturedPiece)
    {
        tempPiecesOnBoard.erase(std::remove(tempPiecesOnBoard.begin(), tempPiecesOnBoard.end(), capturedPiece), tempPiecesOnBoard.end());
    }

    m_piece->Move(move);

    if (!checker->IsCheck(m_piece->get_ColorAndType().get_Color(), tempPiecesOnBoard))
    {
        filteredMoves.emplace_back(move);
    }

    return filteredMoves;
}

std::vector<Chess::Coordinate> Chess::MoveChecker::GetPossibleMoves(const std::vector<std::shared_ptr<IPiece>>& piecesOnBoard)
{
    std::vector<Chess::Coordinate> filteredMoves;
    auto notFilteredMoves = m_moveCheckerOfPiece->GetMoves(m_piece, piecesOnBoard);
    auto pieceCoordinate = m_piece->get_Position();

    for (const auto& move : notFilteredMoves)
    {
        auto partOfFilteredMoves = EmulateCheckIfMove(move, piecesOnBoard);
        filteredMoves.insert(filteredMoves.end(), partOfFilteredMoves.begin(), partOfFilteredMoves.end());
    }

    m_piece->Move(pieceCoordinate);

    return filteredMoves;
}
