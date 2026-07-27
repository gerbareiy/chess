module;
#include <memory>
#include <print>
#include <ranges>
export module Chess.Console.ChessboardPresenter;
import Chess.Console.ConsoleScreen;
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
    export class ChessboardPresenter
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
            chessboard_->onChessboardUpdated.Add(&ChessboardPresenter::Show, *this);
        }

        ~ChessboardPresenter()
        {
            if (chessboard_)
            {
                chessboard_->onChessboardUpdated.Remove(&ChessboardPresenter::Show, *this);
            }
        }

        void Show() const
        {
            ConsoleScreen::Clear();
            ShowTakenPieces(Core::ePieceColor::WHITE);
            ShowChessboardWithCoordinates();
            ShowTakenPieces(Core::ePieceColor::BLACK);
        }

        void ShowChessboardRowWithRank(int y) const
        {
            for (const char x : std::views::iota('A', 'A' + Constants::Sizes::CHESSBOARD_SIZE))
            {
                auto       colorAndType = chessboard_->GetPieceDirector()->GetPieceColorAndType({ .file = x, .rank = y });
                const auto textColor    = GetTextConsoleColor(colorAndType);
                const auto background   = GetBackgroundConsoleColor(Core::Coordinate(x, y));

                ConsoleScreen::SetColor(textColor, background);

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

        static eConsoleColor GetTextConsoleColor(const Core::PieceColorAndType& colorAndType)
        {
            if (colorAndType.color == Core::ePieceColor::BLACK)
            {
                return eConsoleColor::BLACK;
            }
            if (colorAndType.color == Core::ePieceColor::WHITE)
            {
                return eConsoleColor::WHITE;
            }
            return eConsoleColor::DEFAULT;
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
            constexpr auto isChessboardSizeOneDigit = Constants::Sizes::CHESSBOARD_SIZE < 10;

            ShowChessboardFiles(isChessboardSizeOneDigit);

            for (auto y = Constants::Sizes::CHESSBOARD_SIZE; y > 0; --y)
            {
                ShowChessboardRank(y, isChessboardSizeOneDigit);
                ShowChessboardRowWithRank(y);
                ConsoleScreen::ResetColor();
                ShowChessboardRank(y, isChessboardSizeOneDigit);
                PrintEmpty();
            }

            ShowChessboardFiles(isChessboardSizeOneDigit);
        }
    };
} // namespace Chess::Console
