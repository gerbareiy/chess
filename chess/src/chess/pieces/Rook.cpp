#include "Rook.h"

#include "../logic/Coordinate.h"
#include "../logic/ErrorConverter.h"
#include "../logic/Sizes.h"
#include "../logic/eError.h"
#include "King.h"
#include "logic/PieceColorAndType.h"
#include "logic/ePieceColor.h"
#include "logic/ePieceType.h"

#include <stdexcept>

Chess::Rook::Rook(ePieceColor color, int orderNumber)
    : m_canMakeCastling(true)
{
    if (orderNumber < 1 || orderNumber > 2)
    {
        throw std::out_of_range(ErrorConverter::ToString(eError::OUT_OF_COUNT_OF_BISHOP_KNIGHT_ROOK_WITH_ONE_COLOR));
    }

    m_colorAndType = PieceColorAndType(color, ePieceType::ROOK);

    const auto file = orderNumber == 1 ? 'A' : 'A' + CHESSBOARD_SIZE - 1;

    switch (color)
    {
    case ePieceColor::BLACK:
        m_position = Coordinate(file, CHESSBOARD_SIZE);
        break;
    case ePieceColor::WHITE:
        m_position = Coordinate(file, 1);
        break;
    default:
        throw std::out_of_range(ErrorConverter::ToString(eError::OUT_OF_CHESSBOARD));
    }
}

Chess::Rook::Rook(ePieceColor pieceColor, int orderNumber, const std::shared_ptr<King>& king)
    : Rook(pieceColor, orderNumber)
{
    MakeTracking(king);
}

Chess::Rook::Rook(ePieceColor color, const Coordinate& coordinate)
    : Piece(PieceColorAndType(color, ePieceType::ROOK), coordinate)
{
}

Chess::Rook::Rook(ePieceColor color, Coordinate coordinate, const std::shared_ptr<King>& king)
{
    MakeTracking(king);
}

bool Chess::Rook::GetCanMakeCastling() const
{
    return m_canMakeCastling;
}

void Chess::Rook::DisableCastling()
{
    m_canMakeCastling = false;
}

void Chess::Rook::MakeTracking(const std::shared_ptr<King>& king)
{
    if (!king)
    {
        return;
    }

    m_castlingConnection = king->ConnectCastling(std::bind(&Rook::OnCastling, this, std::placeholders::_1, std::placeholders::_2));
}

void Chess::Rook::OnCastling(const Coordinate& to, eCastleSide side)
{
    if (GetCanMakeCastling())
    {
        if (GetPosition().GetFile() == 'A' && side == eCastleSide::LEFT)
        {
            Move(Coordinate(to.GetFile() + 1, to.GetRank()));
        }
        else if (GetPosition().GetFile() == 'A' + CHESSBOARD_SIZE - 1 && side == eCastleSide::RIGHT)
        {
            Move(Coordinate(to.GetFile() - 1, to.GetRank()));
        }
    }

    m_castlingConnection.disconnect();
}

void Chess::Rook::Move(Coordinate to, bool isRealMove)
{
    if (isRealMove)
    {
        DisableCastling();
    }

    Piece::Move(to);
}
