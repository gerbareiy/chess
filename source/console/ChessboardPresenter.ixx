module;
#include <boost/signals2/connection.hpp>

#include <windows.h>

#include <functional>
#include <memory>
#include <print>
export module Console.Chess.ChessboardPresenter;
import Chess.Chessboard;
import Chess.Coordinate;
import Chess.ePieceColor;
import Chess.MoveValidator;
import Chess.PieceColorAndType;
import Chess.PieceDirector;
import Chess.PieceTypeConverter;
import Chess.Sizes;
import Console.Chess.ConsoleColor;

namespace Console::Chess
{
    export class ChessboardPresenter : public std::enable_shared_from_this<ChessboardPresenter>
    {
        std::shared_ptr<::Chess::Chessboard> m_chessboard;

        boost::signals2::scoped_connection m_connection;

        static std::string GetChessboardFiles()
        {
            std::string files = "";

            for (auto i = 0; i < ::Chess::CHESSBOARD_SIZE; ++i)
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

        static eConsoleColor GetTextConsoleColor(const ::Chess::PieceColorAndType& colorAndType, int originalTextColor)
        {
            if (colorAndType.color == ::Chess::ePieceColor::BLACK)
            {
                return eConsoleColor::BLACK;
            }
            if (colorAndType.color == ::Chess::ePieceColor::WHITE)
            {
                return eConsoleColor::WHITE;
            }
            return static_cast<eConsoleColor>(originalTextColor);
        }

        static void SetConsoleColor(eConsoleColor textColor, eConsoleColor backgroundColor)
        {
            const auto hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, static_cast<int>(backgroundColor) << 4 | static_cast<int>(textColor));
        }

        static void ShowChessboardFiles(bool isChessboardSizeOneDigit)
        {
            PrintEmpty();

            std::print("{}", isChessboardSizeOneDigit ? "   " : "\t");
            std::print("{}", GetChessboardFiles());

            PrintEmpty();
            PrintEmpty();
        }

        static void ShowChessboardRank(int y, bool isChessboardSizeOneDigit)
        {
            const auto space = isChessboardSizeOneDigit ? ' ' : '\t';
            std::print("{}{}{}", space, y, space);
        }

        eConsoleColor GetBackgroundConsoleColor(const ::Chess::Coordinate& coordinate) const
        {
            if (coordinate == m_chessboard->GetFrom())
            {
                return eConsoleColor::BROWN;
            }
            if (coordinate == m_chessboard->GetTo())
            {
                return eConsoleColor::YELLOW;
            }

            const auto isSquareBlack = (coordinate.file + 1 + coordinate.rank) % 2;
            if (m_chessboard->GetMoveValidator()->IsCoordinateInPieceCanMove(coordinate))
            {
                return isSquareBlack ? eConsoleColor::BLUE : eConsoleColor::CERULEAN;
            }
            if (m_chessboard->GetMoveValidator()->IsCoordinateInPossibleMoves(coordinate))
            {
                return isSquareBlack ? eConsoleColor::DARK_RED : eConsoleColor::RED;
            }
            return isSquareBlack ? eConsoleColor::GRAY : eConsoleColor::GREEN;
        }

        void ShowChessboardWithCoordinates() const
        {
            WORD originalColors;
            GetOriginalConsoleColor(originalColors);

            auto           originalTextColor        = originalColors & 0x0F;
            auto           originalBackgroundColor  = (originalColors & 0xF0) >> 4;
            constexpr auto isChessboardSizeOneDigit = ::Chess::CHESSBOARD_SIZE < 10;

            ShowChessboardFiles(isChessboardSizeOneDigit);

            for (auto y = ::Chess::CHESSBOARD_SIZE; y > 0; --y, PrintEmpty())
            {
                ShowChessboardRank(y, isChessboardSizeOneDigit);
                ShowChessboardRowWithRank(y, originalTextColor);
                SetConsoleColor(static_cast<eConsoleColor>(originalTextColor), static_cast<eConsoleColor>(originalBackgroundColor));
                ShowChessboardRank(y, isChessboardSizeOneDigit);
            }

            ShowChessboardFiles(isChessboardSizeOneDigit);
        }

    public:
        static void PrintEmpty()
        {
            std::print("\n");
        }

        explicit ChessboardPresenter(const std::shared_ptr<::Chess::Chessboard>& chessboard)
            : m_chessboard(chessboard)
        {
        }

        void Init()
        {
            if (m_chessboard)
            {
                auto const subscriber = [weak = weak_from_this()]
                {
                    if (const auto shared = weak.lock())
                    {
                        shared->Show();
                    }
                };
                m_connection = m_chessboard->ConnectChessboardUpdated(subscriber);
            }
        }

        // You can choose this default Display
        void Show() const
        {
            system("CLS");
            ShowTakenPieces(::Chess::ePieceColor::WHITE);
            ShowChessboardWithCoordinates();
            ShowTakenPieces(::Chess::ePieceColor::BLACK);
        }
        // Or create own Display by combining some methods below
        void ShowChessboardRowWithRank(int y, int originalTextColor) const
        {
            for (auto x = 'A'; x < 'A' + ::Chess::CHESSBOARD_SIZE; ++x)
            {
                auto       colorAndType = m_chessboard->GetPieceDirector()->GetPieceColorAndType({ .file = x, .rank = y });
                const auto textColor    = GetTextConsoleColor(colorAndType, originalTextColor);
                const auto background   = GetBackgroundConsoleColor(::Chess::Coordinate(x, y));

                SetConsoleColor(textColor, background);

                std::print("{}", ::Chess::PieceTypeConverter::ConvertToShortString(colorAndType.type)[0]);
            }
        }

        void ShowTakenPieces(::Chess::ePieceColor color) const
        {
            const auto eatenPieces = m_chessboard->GetPieceDirector()->GetEatenPieces();

            PrintEmpty();

            for (const auto& piece : eatenPieces)
            {
                if (piece->GetColorAndType().color == color)
                {
                    std::print("{}", ::Chess::PieceTypeConverter::ConvertToShortString(piece->GetColorAndType().type));
                }
            }

            PrintEmpty();
            PrintEmpty();
        }
    };
} // namespace Console::Chess
