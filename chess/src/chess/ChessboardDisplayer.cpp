#include "ChessboardDisplayer.h"

#include "Chessboard.h"
#include "logic/Coordinate.h"
#include "logic/MoveValidator.h"
#include "logic/PieceDirector.h"
#include "logic/Sizes.h"
#include "pieces/Piece.h"
#include "pieces/logic/PieceColorAndType.h"
#include "pieces/logic/PieceTypeConverter.h"

#include <iostream>

std::string Chess::ChessboardDisplayer::GetChessboardFiles()
{
    std::string files = "";

    for (auto i = 0; i < CHESSBOARD_SIZE; ++i)
    {
        files += static_cast<char>('A' + i);
    }

    return files;
}

void Chess::ChessboardDisplayer::GetOriginalConsoleColor(WORD& originalColors)
{
    const auto                 handleConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(handleConsole, &consoleInfo);
    originalColors = consoleInfo.wAttributes;
}

Console::eConsoleColor Chess::ChessboardDisplayer::GetTextConsoleColor(const PieceColorAndType& colorAndType, int originalTextColor)
{
    return colorAndType.GetColor() == ePieceColor::BLACK
               ? Console::eConsoleColor::BLACK
               : (colorAndType.GetColor() == ePieceColor::WHITE ? Console::eConsoleColor::WHITE : static_cast<Console::eConsoleColor>(originalTextColor));
}

void Chess::ChessboardDisplayer::SetConsoleColor(Console::eConsoleColor textColor, Console::eConsoleColor backgroundColor)
{
    const auto hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (static_cast<int>(backgroundColor) << 4) | static_cast<int>(textColor));
}

void Chess::ChessboardDisplayer::ShowChessboardFiles(bool isChessboardSizeOneDigit)
{
    ShowEmpty();

    std::cout << (isChessboardSizeOneDigit ? "   " : "\t");
    std::cout << GetChessboardFiles();

    ShowEmpty();
    ShowEmpty();
}

void Chess::ChessboardDisplayer::ShowChessboardRank(int y, bool isChessboardSizeOneDigit)
{
    const auto space = (isChessboardSizeOneDigit ? ' ' : '\t');
    std::cout << space << y << space;
}

Console::eConsoleColor Chess::ChessboardDisplayer::GetBackgroundConsoleColor(const Coordinate& coordinate) const
{
    Console::eConsoleColor color;
    const auto             isSquareBlack = ((coordinate.GetFile() + 1) + coordinate.GetRank()) % 2;

    if (coordinate == m_chessboard->GetFrom())
    {
        color = Console::eConsoleColor::BROWN;
    }
    else if (coordinate == m_chessboard->GetTo())
    {
        color = Console::eConsoleColor::YELLOW;
    }
    else if (m_chessboard->GetMoveValidator()->IsCoordinateInPieceCanMove(coordinate))
    {
        color = isSquareBlack ? Console::eConsoleColor::BLUE : Console::eConsoleColor::CERULEAN;
    }
    else if (m_chessboard->GetMoveValidator()->IsCoordinateInPossibleMoves(coordinate))
    {
        color = isSquareBlack ? Console::eConsoleColor::DARK_RED : Console::eConsoleColor::RED;
    }
    else
    {
        color = isSquareBlack ? Console::eConsoleColor::GRAY : Console::eConsoleColor::GREEN;
    }

    return color;
}

void Chess::ChessboardDisplayer::ShowChessboardWithCoordinates() const
{
    WORD originalColors;
    GetOriginalConsoleColor(originalColors);

    auto           originalTextColor        = originalColors & 0x0F;
    auto           originalBackgroundColor  = (originalColors & 0xF0) >> 4;
    constexpr auto isChessboardSizeOneDigit = CHESSBOARD_SIZE < 10;

    ShowChessboardFiles(isChessboardSizeOneDigit);

    for (auto y = CHESSBOARD_SIZE; y > 0; --y, ShowEmpty())
    {
        ShowChessboardRank(y, isChessboardSizeOneDigit);
        ShowChessboardRowWithRank(y, originalTextColor);
        SetConsoleColor(static_cast<Console::eConsoleColor>(originalTextColor), static_cast<Console::eConsoleColor>(originalBackgroundColor));
        ShowChessboardRank(y, isChessboardSizeOneDigit);
    }

    ShowChessboardFiles(isChessboardSizeOneDigit);
}

Chess::ChessboardDisplayer::ChessboardDisplayer(const std::shared_ptr<Chessboard>& chessboard)
    : m_chessboard(chessboard)
{
    if (m_chessboard)
    {
        m_chessboard->ConnectChessboardUpdated(std::bind(&ChessboardDisplayer::Show, this));
    }
}

void Chess::ChessboardDisplayer::ShowChessboardRowWithRank(int y, int originalTextColor) const
{
    for (auto x = 'A'; x < 'A' + CHESSBOARD_SIZE; ++x)
    {
        auto       colorAndType = m_chessboard->GetPieceDirector()->GetPieceColorAndType(Coordinate(x, y));
        const auto textColor    = GetTextConsoleColor(colorAndType, originalTextColor);
        const auto background   = GetBackgroundConsoleColor(Coordinate(x, y));

        SetConsoleColor(textColor, background);

        std::cout << PieceTypeConverter::ConvertToString(colorAndType.GetType())[0];
    }
}

void Chess::ChessboardDisplayer::Show()
{
    system("CLS");
    ShowTakenPieces(ePieceColor::WHITE);
    ShowChessboardWithCoordinates();
    ShowTakenPieces(ePieceColor::BLACK);
}

void Chess::ChessboardDisplayer::ShowEmpty()
{
    std::cout << '\n';
}

void Chess::ChessboardDisplayer::ShowTakenPieces(ePieceColor color) const
{
    const auto eatenPieces = m_chessboard->GetPieceDirector()->GetEatenPieces();

    ShowEmpty();

    for (const auto& piece : eatenPieces)
    {
        if (piece->GetColorAndType().GetColor() == color)
        {
            std::cout << PieceTypeConverter::ConvertToString(piece->GetColorAndType().GetType());
        }
    }

    ShowEmpty();
    ShowEmpty();
}
