#include "Pawn.h"

#include "core/logic/Coordinate.h"
#include "core/logic/ErrorConverter.h"
#include "core/logic/PieceSignalDirector.h"
#include "core/logic/Sizes.h"
#include "core/logic/eError.h"
#include "logic/PieceColorAndType.h"

#include <stdexcept>

void Chess::Pawn::LostEnPassant()
{
    m_canEnPassant = false;
}

void Chess::Pawn::MakeTracking(const std::shared_ptr<PieceSignalDirector>& signalDirector)
{
    if (!signalDirector)
    {
        return;
    }

    signalDirector->ConnectMove(
        [this]()
        {
            if (!m_isOnPawnFirstMove)
            {
                LostEnPassant();
            }

            m_isOnPawnFirstMove = false;
        });
}

Chess::Pawn::Pawn(ePieceColor color, char file)
{
    if (file < 'A' || file > 'A' + CHESSBOARD_SIZE - 1)
    {
        throw std::out_of_range(ErrorConverter::ToString(eError::OUT_OF_CHESSBOARD));
    }

    m_colorAndType = PieceColorAndType(color, ePieceType::PAWN);

    switch (color)
    {
    case ePieceColor::BLACK:
        m_position = Coordinate(file, CHESSBOARD_SIZE - 1);
        break;
    case ePieceColor::WHITE:
        m_position = Coordinate(file, 2);
        break;
    default:
        throw std::out_of_range(ErrorConverter::ToString(eError::OUT_OF_CHESSBOARD));
    }
}

Chess::Pawn::Pawn(ePieceColor color, char file, const std::shared_ptr<PieceSignalDirector>& signalDirector)
    : Pawn(color, file)
{
    MakeTracking(signalDirector);
}

Chess::Pawn::Pawn(ePieceColor color, const Coordinate& coordinate)
    : Piece(PieceColorAndType(color, ePieceType::PAWN), coordinate)
{
}

Chess::Pawn::Pawn(ePieceColor color, const Coordinate& coordinate, const std::shared_ptr<PieceSignalDirector>& signalDirector)
    : Pawn(color, coordinate)
{
    MakeTracking(signalDirector);
}

bool Chess::Pawn::GetCanEnPassant() const
{
    return m_canEnPassant;
}

bool Chess::Pawn::GetIsNotMoved() const
{
    return m_isNotMoved;
}

void Chess::Pawn::Move(Coordinate to, bool isRealMove)
{
    if (isRealMove)
    {
        if (abs(to.rank - GetPosition().rank) == 2)
        {
            m_canEnPassant = m_isNotMoved;
        }
        else
        {
            LostEnPassant();
        }

        m_isOnPawnFirstMove = m_isNotMoved;
        m_isNotMoved        = false;
    }

    Piece::Move(to);
}
