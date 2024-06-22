#include "MoveChecker.h"

#include "CheckChecker.h"
#include "MoveCheckerFactory.h"
#include "PieceFinder.h"

#include "stdexcept"
#include <vector>

Chess::MoveChecker::MoveChecker(const std::shared_ptr<IPiece>& piece)
    : m_piece(piece), m_moveChecker(std::unique_ptr<MoveCheckerFactory>()->Create(piece)) { }

std::vector<Chess::Coordinate> Chess::MoveChecker::GetPossibleMoves(const std::vector<std::shared_ptr<IPiece>>& piecesOnBoard)
{
    const auto checker = std::unique_ptr<CheckChecker>();
    std::vector<Chess::Coordinate> filteredMoves;
    auto notFilteredMoves = m_moveChecker->GetMoves(m_piece, piecesOnBoard);
    auto pieceCoordinate = m_piece->get_Position();
    auto finder = std::make_shared<PieceFinder>(piecesOnBoard);

    for (const auto& move : notFilteredMoves)
    {
        auto capturedPiece = finder->Find(move);

        if (capturedPiece)
        {
            capturedPiece->Move(Coordinate( 0, 0 ));
        }

        m_piece->Move(move);

        if (!checker->IsCheck(m_piece->get_ColorAndType().get_Color(), piecesOnBoard))
        {
            filteredMoves.emplace_back(move);
        }

        if (capturedPiece)
        {
            capturedPiece->Move(move);
        }
    }

    m_piece->Move(pieceCoordinate);

    return filteredMoves;
}
