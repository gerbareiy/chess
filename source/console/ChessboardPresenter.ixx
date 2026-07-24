module;
#include <boost/signals2/connection.hpp>
#include <functional>
#include <memory>
#include <print>
#include <ranges>
#include <windows.h>
export module Chess.Console.ChessboardPresenter;
import Chess.Console.eConsoleColor;
import Chess.Constants.Sizes;
import Chess.Core.Chessboard;
import Chess.Core.Coordinate;
import Chess.Core.ePieceColor;
import Chess.Core.MoveValidator;
import Chess.Core.PieceColorAndType;
import Chess.Core.PieceDirector;
import Chess.Core.PieceTypeConverter;

namespace Chess::Console
{
    export class ChessboardPresenter : public std::enable_shared_from_this<ChessboardPresenter>
    {
    public:
        explicit ChessboardPresenter(const std::shared_ptr<Core::Chessboard>& chessboard)
            : chessboard_(chessboard)
        {
        }

        void Init()
        {
            if (!chessboard_)
            {
                return;
            }
            auto const subscriber = [weak = weak_from_this()]
            {
                if (const auto shared = weak.lock())
                {
                    shared->Show();
                }
            };
            connection_ = chessboard_->ConnectOnChessboardUpdated(subscriber);
        }

        void Show() const
        {
            Clear();
            ShowTakenPieces(Core::ePieceColor::WHITE);
            ShowChessboardWithCoordinates();
            ShowTakenPieces(Core::ePieceColor::BLACK);
        }

        void ShowChessboardRowWithRank(int y, int originalTextColor) const
        {
            for (const char x : std::views::iota('A', 'A' + Constants::Sizes::CHESSBOARD_SIZE))
            {
                auto       colorAndType = chessboard_->GetPieceDirector()->GetPieceColorAndType({ .file = x, .rank = y });
                const auto textColor    = GetTextConsoleColor(colorAndType, originalTextColor);
                const auto background   = GetBackgroundConsoleColor(Core::Coordinate(x, y));

                SetConsoleColor(textColor, background);

                std::print("{}", Core::PieceTypeConverter::TryConvertToChar(colorAndType.type).value_or(' '));
            }
        }

        void ShowTakenPieces(Core::ePieceColor color) const
        {
            PrintEmpty();

            const auto eatenPieces = chessboard_->GetPieceDirector()->GetEatenPieces();
            for (const auto& piece : eatenPieces)
            {
                if (piece->GetColorAndType().color == color)
                {
                    std::print("{}", Core::PieceTypeConverter::TryConvertToChar(piece->GetColorAndType().type).value_or(' '));
                }
            }

            PrintEmpty();
            PrintEmpty();
        }

    private:
        std::shared_ptr<Core::Chessboard> chessboard_;

        boost::signals2::scoped_connection connection_;

        static std::string GetChessboardFiles()
        {
            std::string result = "";
            result.reserve(Constants::Sizes::CHESSBOARD_SIZE);
            for (const char file : std::views::iota('A', 'A' + Constants::Sizes::CHESSBOARD_SIZE))
            {
                result += file;
            }
            return result;
        }

        static void GetOriginalConsoleColor(WORD& originalColors)
        {
            const auto                 handleConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
            GetConsoleScreenBufferInfo(handleConsole, &consoleInfo);
            originalColors = consoleInfo.wAttributes;
        }

        static eConsoleColor GetTextConsoleColor(const Core::PieceColorAndType& colorAndType, int originalTextColor)
        {
            if (colorAndType.color == Core::ePieceColor::BLACK)
            {
                return eConsoleColor::BLACK;
            }
            if (colorAndType.color == Core::ePieceColor::WHITE)
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

        static void Clear()
        {
            system("CLS");
        }

        static void PrintEmpty()
        {
            std::print("\n");
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

        eConsoleColor GetBackgroundConsoleColor(const Core::Coordinate& coordinate) const
        {
            if (coordinate == chessboard_->GetFrom())
            {
                return eConsoleColor::BROWN;
            }
            if (coordinate == chessboard_->GetTo())
            {
                return eConsoleColor::YELLOW;
            }

            const auto isBlackSquare = (coordinate.file + 1 + coordinate.rank) % 2;
            if (chessboard_->GetMoveValidator()->IsCoordinateInPieceCanMove(coordinate))
            {
                return isBlackSquare ? eConsoleColor::BLUE : eConsoleColor::CERULEAN;
            }
            if (chessboard_->GetMoveValidator()->IsCoordinateInPossibleMoves(coordinate))
            {
                return isBlackSquare ? eConsoleColor::DARK_RED : eConsoleColor::RED;
            }
            return isBlackSquare ? eConsoleColor::GRAY : eConsoleColor::GREEN;
        }

        void ShowChessboardWithCoordinates() const
        {
            WORD originalColors;
            GetOriginalConsoleColor(originalColors);

            auto           originalTextColor        = originalColors & 0x0F;
            auto           originalBackgroundColor  = (originalColors & 0xF0) >> 4;
            constexpr auto isChessboardSizeOneDigit = Constants::Sizes::CHESSBOARD_SIZE < 10;

            ShowChessboardFiles(isChessboardSizeOneDigit);

            for (auto y = Constants::Sizes::CHESSBOARD_SIZE; y > 0; --y)
            {
                ShowChessboardRank(y, isChessboardSizeOneDigit);
                ShowChessboardRowWithRank(y, originalTextColor);
                SetConsoleColor(static_cast<eConsoleColor>(originalTextColor), static_cast<eConsoleColor>(originalBackgroundColor));
                ShowChessboardRank(y, isChessboardSizeOneDigit);
                PrintEmpty();
            }

            ShowChessboardFiles(isChessboardSizeOneDigit);
        }
    };
} // namespace Chess::Console
