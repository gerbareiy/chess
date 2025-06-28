#include "PieceColorConverter.h"

std::string Chess::PieceColorConverter::ConvertToString(ePieceColor color)
{
    switch (color)
    {
    case ePieceColor::NONE:
        return " ";
    case ePieceColor::BLACK:
        return "b";
    case ePieceColor::WHITE:
        return "w";
    default:
        return "Unknown color";
    }
}
