module;
#include <memory>
#include <vector>
module Chess.MoveValidator;
import Chess.Piece;
import Chess.PieceFinder;
import Chess.Player;

void Chess::MoveValidator::CalculatePossibleMoves(const std::shared_ptr<Piece>& piece)
{
    const auto it = std::ranges::find(m_piecesCanMove, piece);

    if (it != m_piecesCanMove.end())
    {
        const auto moveChecker = std::make_shared<MoveChecker>(piece);
        m_possibleMoves        = moveChecker->GetFilteredMoves(m_piecesOnBoard);
    }
}

void Chess::MoveValidator::ClearPossibleMoves()
{
    m_possibleMoves.clear();
}

void Chess::MoveValidator::ClearPiecesCanMove()
{
    m_piecesCanMove.clear();
}

size_t Chess::MoveValidator::GetPiecesCanMoveCount()
{
    return m_piecesCanMove.size();
}

bool Chess::MoveValidator::IsCoordinateInPieceCanMove(const Coordinate& coordinate) const
{
    const auto finder = std::make_unique<PieceFinder>(m_piecesCanMove);

    return !!finder->Find(coordinate);
}

bool Chess::MoveValidator::IsCoordinateInPossibleMoves(const Coordinate& coordinate) const
{
    const auto it = std::ranges::find(m_possibleMoves, coordinate);

    return it != m_possibleMoves.end();
}

bool Chess::MoveValidator::IsValidMove(const std::shared_ptr<Piece>& piece, const Coordinate& to) const
{
    if (!piece)
    {
        return false;
    }

    return std::ranges::find(m_possibleMoves, to) != m_possibleMoves.end();
}
