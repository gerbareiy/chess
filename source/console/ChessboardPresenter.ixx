module;
#include <windows.h>

#include <functional>
#include <memory>
#include <print>
export module Chess.ChessboardPresenter;
import Chess.Chessboard;
import Chess.Coordinate;
import Chess.eConsoleColor;
import Chess.ePieceColor;
import Chess.MoveValidator;
import Chess.PieceColorAndType;
import Chess.PieceDirector;
import Chess.PieceTypeConverter;
import Chess.Sizes;

namespace Chess
{
    export class ChessboardPresenter
    {
        std::shared_ptr<Chessboard> m_chessboard;

        static std::string GetChessboardFiles()
        {
            std::string files = "";

            for (auto i = 0; i < CHESSBOARD_SIZE; ++i)
            {
                files += static_cast<char>('A' + i);
            }
            return files;
        }

        static void GetOriginalConsoleColor(WORD& originalColors)
        {
            const auto                 handleConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
            GetConsoleScreenBufferInfo(handleConsole, &consoleInfo);
            originalColors = consoleInfo.wAttributes;
        }

        static Console::eConsoleColor GetTextConsoleColor(const PieceColorAndType& colorAndType, int originalTextColor)
        {
            if (colorAndType.color == ePieceColor::BLACK)
            {
                return Console::eConsoleColor::BLACK;
            }
            if (colorAndType.color == ePieceColor::WHITE)
            {
                return Console::eConsoleColor::WHITE;
            }
            return static_cast<Console::eConsoleColor>(originalTextColor);
        }

        static void SetConsoleColor(Console::eConsoleColor textColor, Console::eConsoleColor backgroundColor)
        {
            const auto hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, (static_cast<int>(backgroundColor) << 4) | static_cast<int>(textColor));
        }

        static void ShowChessboardFiles(bool isChessboardSizeOneDigit)
        {
            ShowEmpty();

            std::print("{}", isChessboardSizeOneDigit ? "   " : "\t");
            std::print("{}", GetChessboardFiles());

            ShowEmpty();
            ShowEmpty();
        }

        static void ShowChessboardRank(int y, bool isChessboardSizeOneDigit)
        {
            const auto space = (isChessboardSizeOneDigit ? ' ' : '\t');
            std::print("{}{}{}", space, y, space);
        }

        Console::eConsoleColor GetBackgroundConsoleColor(const Coordinate& coordinate) const
        {
            if (coordinate == m_chessboard->GetFrom())
            {
                return Console::eConsoleColor::BROWN;
            }
            if (coordinate == m_chessboard->GetTo())
            {
                return Console::eConsoleColor::YELLOW;
            }

            const auto isSquareBlack = (coordinate.file + 1 + coordinate.rank) % 2;
            if (m_chessboard->GetMoveValidator()->IsCoordinateInPieceCanMove(coordinate))
            {
                return isSquareBlack ? Console::eConsoleColor::BLUE : Console::eConsoleColor::CERULEAN;
            }
            if (m_chessboard->GetMoveValidator()->IsCoordinateInPossibleMoves(coordinate))
            {
                return isSquareBlack ? Console::eConsoleColor::DARK_RED : Console::eConsoleColor::RED;
            }
            return isSquareBlack ? Console::eConsoleColor::GRAY : Console::eConsoleColor::GREEN;
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
            std::print("\n");
        }

        explicit ChessboardPresenter(const std::shared_ptr<Chessboard>& chessboard)
            : m_chessboard(chessboard)
        {
            if (m_chessboard)
            {
                m_chessboard->ConnectChessboardUpdated(std::bind(&ChessboardPresenter::Show, this));
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

                std::print("{}", PieceTypeConverter::ConvertToShortString(colorAndType.type)[0]);
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
                    std::print("{}", PieceTypeConverter::ConvertToShortString(piece->GetColorAndType().type));
                }
            }

            ShowEmpty();
            ShowEmpty();
        }
    };
} // namespace Chess
