module;
#include <algorithm>
#include <boost/signals2.hpp>

#include <memory>
#include <typeinfo>
module Chess.PieceDirector;
import Chess.CheckChecker;
import Chess.Counts;
import Chess.ePieceColor;
import Chess.Pawn;
import Chess.PieceSignalDirector;
import Chess.PieceTakeLocator;
import Chess.Promotion;
import Chess.Sizes;

void Chess::PieceDirector::Take(int indexOnBoard)
{
    m_eatenPieces.emplace_back(std::move(m_piecesOnBoard[indexOnBoard]));
    m_piecesOnBoard.erase(m_piecesOnBoard.begin() + indexOnBoard);
}

Chess::PieceDirector::PieceDirector(const std::vector<std::shared_ptr<Piece>>& piecesOnBoard, const std::shared_ptr<PieceSignalDirector>& signalDirector)
    : m_piecesOnBoard(piecesOnBoard)
    , m_signalDirector(signalDirector)
{
    m_eatenPieces.reserve(MAX_COUNT_ELEMENTS);
    m_promotion = std::make_shared<Promotion>();
}

const std::shared_ptr<Chess::Piece>& Chess::PieceDirector::GetCurrentPiece() const
{
    return m_currentPiece;
}

const std::vector<std::shared_ptr<Chess::Piece>>& Chess::PieceDirector::GetEatenPieces() const
{
    return m_eatenPieces;
}

bool Chess::PieceDirector::GetIsCheck() const
{
    return m_isCheck;
}

std::shared_ptr<Chess::Piece> Chess::PieceDirector::GetPiece(const Coordinate& from) const
{
    for (std::shared_ptr<Piece> pieceOnBoard : m_piecesOnBoard)
    {
        if (pieceOnBoard->GetPosition() == from)
        {
            return pieceOnBoard;
        }
    }

    return nullptr;
}

void Chess::PieceDirector::InitCurrentPiece(const Coordinate& from)
{
    m_currentPiece = GetPiece(from);
}

void Chess::PieceDirector::MovePiece(const Coordinate& to, const boost::signals2::signal<void()>& signalChessboardUndated)
{
    auto fromTake = PieceTakeLocator::Find(m_currentPiece, m_piecesOnBoard, to);

    const auto it = std::ranges::find_if(m_piecesOnBoard,
                                         [fromTake](const std::shared_ptr<Piece>& current)
                                         {
                                             return current->GetPosition() == fromTake;
                                         });

    if (it != m_piecesOnBoard.end())
    {
        Take(static_cast<int>(std::distance(m_piecesOnBoard.begin(), it)));
    }

    m_currentPiece->Move(to);
    m_signalDirector->Invite();

    if (typeid(*m_currentPiece) == typeid(Pawn)
        && (m_currentPiece->GetPosition().rank == 1 && m_currentPiece->GetColorAndType().color == ePieceColor::BLACK
            || m_currentPiece->GetPosition().rank == CHESSBOARD_SIZE && m_currentPiece->GetColorAndType().color == ePieceColor::WHITE))
    {
        signalChessboardUndated();
        Promotion::PromoteConditionally(std::static_pointer_cast<Pawn>(m_currentPiece), m_piecesOnBoard);
    }

    auto color = ePieceColor::NONE;
    if (m_currentPiece->GetColorAndType().color == ePieceColor::BLACK)
    {
        color = ePieceColor::WHITE;
    }
    else if (m_currentPiece->GetColorAndType().color == ePieceColor::WHITE)
    {
        color = ePieceColor::BLACK;
    }

    m_isCheck = CheckChecker::IsCheck(color, m_piecesOnBoard);
    m_signalDirector->Invite(m_isCheck);
}
