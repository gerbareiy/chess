module;
#include <windows.h>

#include <functional>
#include <iostream>
#include <memory>
export module Chess.ChessboardDisplayer;
import Chess.Chessboard;
import Chess.Coordinate;
import Chess.eConsoleColor;
import Chess.ePieceColor;
import Chess.MoveValidator;
import Chess.PieceColorAndType;
import Chess.PieceDirector;
import Chess.PieceTypeConverter;
import Chess.Sizes;

std::string GetChessboardFiles()
{
    std::string files = "";

    for (auto i = 0; i < Chess::CHESSBOARD_SIZE; ++i)
    {
        files += static_cast<char>('A' + i);
    }

    return files;
}

void GetOriginalConsoleColor(WORD& originalColors)
{
    const auto                 handleConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(handleConsole, &consoleInfo);
    originalColors = consoleInfo.wAttributes;
}

Console::eConsoleColor GetTextConsoleColor(const Chess::PieceColorAndType& colorAndType, int originalTextColor)
{
    return colorAndType.color == Chess::ePieceColor::BLACK   ? Console::eConsoleColor::BLACK
           : colorAndType.color == Chess::ePieceColor::WHITE ? Console::eConsoleColor::WHITE
                                                             : static_cast<Console::eConsoleColor>(originalTextColor);
}

void SetConsoleColor(Console::eConsoleColor textColor, Console::eConsoleColor backgroundColor)
{
    const auto hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (static_cast<int>(backgroundColor) << 4) | static_cast<int>(textColor));
}

void PrintEmpty()
{
    std::cout << '\n';
}

void ShowChessboardFiles(bool isChessboardSizeOneDigit)
{
    PrintEmpty();

    std::cout << (isChessboardSizeOneDigit ? "   " : "\t");
    std::cout << GetChessboardFiles();

    PrintEmpty();
    PrintEmpty();
}

void ShowChessboardRank(int y, bool isChessboardSizeOneDigit)
{
    const auto space = (isChessboardSizeOneDigit ? ' ' : '\t');
    std::cout << space << y << space;
}

namespace Chess
{
    export class ChessboardDisplayer
    {
        std::shared_ptr<Chessboard> m_chessboard;

        Console::eConsoleColor GetBackgroundConsoleColor(const Coordinate& coordinate) const
        {
            Console::eConsoleColor color;
            const auto             isSquareBlack = (coordinate.file + 1 + coordinate.rank) % 2;

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

        void ShowChessboardWithCoordinates() const
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

    public:
        static void ShowEmpty()
        {
            PrintEmpty();
        }

        explicit ChessboardDisplayer(const std::shared_ptr<Chessboard>& chessboard)
            : m_chessboard(chessboard)
        {
            if (m_chessboard)
            {
                m_chessboard->ConnectChessboardUpdated(std::bind(&ChessboardDisplayer::Show, this));
            }
        }

        // You can choose this default Display
        void Show()
        {
            system("CLS");
            ShowTakenPieces(ePieceColor::WHITE);
            ShowChessboardWithCoordinates();
            ShowTakenPieces(ePieceColor::BLACK);
        }
        // Or create own Display by combining some methods below
        void ShowChessboardRowWithRank(int y, int originalTextColor) const
        {
            for (auto x = 'A'; x < 'A' + CHESSBOARD_SIZE; ++x)
            {
                auto       colorAndType = m_chessboard->GetPieceDirector()->GetPieceColorAndType({ .file = x, .rank = y });
                const auto textColor    = GetTextConsoleColor(colorAndType, originalTextColor);
                const auto background   = GetBackgroundConsoleColor(Coordinate(x, y));

                SetConsoleColor(textColor, background);

                std::cout << PieceTypeConverter::ConvertToString(colorAndType.type)[0];
            }
        }

        void ShowTakenPieces(ePieceColor color) const
        {
            const auto eatenPieces = m_chessboard->GetPieceDirector()->GetEatenPieces();

            ShowEmpty();

            for (const auto& piece : eatenPieces)
            {
                if (piece->GetColorAndType().color == color)
                {
                    std::cout << PieceTypeConverter::ConvertToString(piece->GetColorAndType().type);
                }
            }

            ShowEmpty();
            ShowEmpty();
        }
    };
} // namespace Chess
