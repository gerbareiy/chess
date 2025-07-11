#include "King.h"

#include "core/logic/Coordinate.h"
#include "core/logic/ErrorConverter.h"
#include "core/logic/PieceSignalDirector.h"
#include "core/logic/Sizes.h"
#include "core/logic/eError.h"
#include "logic/PieceColorAndType.h"
#include <math.h>

#include <stdexcept>

void Chess::King::DisableCastling()
{
    m_canMakeCastling = false;
}

void Chess::King::MakeTracking(const std::shared_ptr<PieceSignalDirector>& signalDirector)
{
    if (!signalDirector)
    {
        return;
    }

    signalDirector->ConnectMoveWithCheck(
        [this](bool isCheck)
        {
            m_isCheck = isCheck;
        });
}

Chess::King::King(ePieceColor color)
{
    m_colorAndType = PieceColorAndType(color, ePieceType::KING);

    switch (color)
    {
    case ePieceColor::BLACK:
        m_position = Coordinate('E', CHESSBOARD_SIZE);
        break;
    case ePieceColor::WHITE:
        m_position = Coordinate('E', 1);
        break;
    default:
        throw std::out_of_range(ErrorConverter::ToString(eError::OUT_OF_CHESSBOARD));
    }
}

Chess::King::King(ePieceColor color, const std::shared_ptr<PieceSignalDirector>& signalDirector)
    : King(color)
{
    MakeTracking(signalDirector);
}

Chess::King::King(ePieceColor color, const Coordinate& coordinate)
    : Piece(PieceColorAndType(color, ePieceType::KING), coordinate)
{
}

Chess::King::King(ePieceColor color, const Coordinate& coordinate, const std::shared_ptr<PieceSignalDirector>& signalDirector)
    : King(color, coordinate)
{
    MakeTracking(signalDirector);
}

bool Chess::King::GetCanMakeCastling() const
{
    return m_canMakeCastling;
}

bool Chess::King::GetIsCheck() const
{
    return m_isCheck;
}

void Chess::King::Move(Coordinate to, bool isRealMove)
{
    if (isRealMove)
    {
        DisableCastling();

        if (abs(GetPosition().GetFile() - to.GetFile()) > 1)
        {
            eCastleSide side;

            if (to.GetFile() > GetPosition().GetFile())
            {
                side = eCastleSide::RIGHT;
            }
            else if (to.GetFile() < GetPosition().GetFile())
            {
                side = eCastleSide::LEFT;
            }
            else
            {
                throw std::invalid_argument(ErrorConverter::ToString(eError::NOT_CORRECT_MOVE));
            }

            m_signalCastling(to, side);
        }
    }

    Piece::Move(to);
}

boost::signals2::connection Chess::King::ConnectCastling(const boost::signals2::signal<void(Coordinate, eCastleSide)>::slot_type& subscriber)
{
    return m_signalCastling.connect(subscriber);
}
