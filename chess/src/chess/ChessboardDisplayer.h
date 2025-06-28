#pragma once

#include "logic/eConsoleColor.h"
#include "pieces/logic/ePieceColor.h"
#include <windows.h>

#include <memory>
#include <string>

namespace Chess
{
    class Chessboard;
    struct Coordinate;
    struct PieceColorAndType;

    class ChessboardDisplayer
    {
    private:
        const std::shared_ptr<Chessboard> m_chessboard;

    public:
        ChessboardDisplayer(const std::shared_ptr<Chessboard>& chessboard);

    private:
        Console::eConsoleColor        GetBackgroundConsoleColor(const Coordinate& coordinate) const;
        static std::string            GetChessboardFiles();
        static void                   GetOriginalConsoleColor(WORD& originalColors);
        static Console::eConsoleColor GetTextConsoleColor(const PieceColorAndType& colorAndType, int originalTextColor);
        static void                   SetConsoleColor(Console::eConsoleColor textColor, Console::eConsoleColor backgroundColor);
        static void                   ShowChessboardFiles(bool isChessboardSizeOneDigit);
        static void                   ShowChessboardRank(int y, bool isChessboardSizeOneDigit);
        void                          ShowChessboardWithCoordinates() const;

    public:
        // You can choose this default Display
        void Show();
        // Or create own Display by combining some methiods below
        void        ShowChessboardRowWithRank(int y, int originalTextColor) const;
        static void ShowEmpty();
        void        ShowTakenPieces(ePieceColor color) const;
    };
} // namespace Chess
