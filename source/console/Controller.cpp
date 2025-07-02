#include "Controller.h"

#include "core/Chessboard.h"

Chess::Controller::Controller(const std::shared_ptr<Chessboard>& chessboard)
    : m_chessboard(chessboard)
{
}

bool Chess::Controller::TryInitPiece(const Coordinate& from) const
{
    return m_chessboard->TryInitPiece(from);
}

bool Chess::Controller::TryMovePiece(const Coordinate& to) const
{
    const auto isMoved = m_chessboard->TryMovePiece(to);

    m_signalMove();

    return isMoved;
}

boost::signals2::connection Chess::Controller::ConnectMove(const boost::signals2::signal<void()>::slot_type& subscriber)
{
    return m_signalMove.connect(subscriber);
}
