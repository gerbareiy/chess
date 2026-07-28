module;
#include <cstddef>
#include <memory>
#include <ranges>
export module Chess.Core.DrawChecker;
import Chess.Constants.Counts;
import Chess.Constants.Sizes;
import Chess.Core.Chessboard;
import Chess.Core.Coordinate;
import Chess.Core.ePieceColor;
import Chess.Core.ePieceType;
import Chess.Core.PieceDirector;

namespace Chess::Core
{
    export class DrawChecker
    {
    public:
        bool IsDraw(const std::shared_ptr<Chessboard>& chessboard)
        {
            RefreshMovesCountWithoutPawnAndTaking(chessboard);

            const bool isStalemate = chessboard->GetMoveValidator()->GetPiecesCanMoveCount() == 0 && !chessboard->GetPieceDirector()->GetIsCheck();

            return isStalemate || halfMovesWithoutPawnMoveAndTaking_ >= Constants::Counts::MAX_HALF_MOVES_WITHOUT_PAWN_MOVE_AND_TAKING_COUNT
                   || IsInsufficientMaterial(chessboard);
        }

    private:
        int    halfMovesWithoutPawnMoveAndTaking_ = 0;
        size_t lastEatenPiecesCount_              = 0;

        static bool IsInsufficientMaterial(const std::shared_ptr<Chessboard>& chessboard)
        {
            auto blackBishopDarkCount  = 0;
            auto blackBishopLightCount = 0;
            auto blackKing             = false;
            auto blackKnightCount      = 0;
            auto whiteBishopDarkCount  = 0;
            auto whiteBishopLightCount = 0;
            auto whiteKing             = false;
            auto whiteKnightCount      = 0;

            for (auto y = Constants::Sizes::CHESSBOARD_SIZE; y > 0; --y)
            {
                for (const char x : std::views::iota('A', 'A' + Constants::Sizes::CHESSBOARD_SIZE))
                {
                    const auto piece = chessboard->GetPieceDirector()->GetPiece(Coordinate(x, y));

                    if (!piece)
                    {
                        continue;
                    }

                    const auto [color, type] = piece->GetColorAndType();

                    switch (type)
                    {
                    case ePieceType::BISHOP:
                        if (color == ePieceColor::BLACK)
                        {
                            (x + y) % 2 == 0 ? ++blackBishopLightCount : ++blackBishopDarkCount;
                        }
                        else
                        {
                            (x + y) % 2 == 0 ? ++whiteBishopLightCount : ++whiteBishopDarkCount;
                        }
                        break;
                    case ePieceType::KNIGHT:
                        color == ePieceColor::BLACK ? ++blackKnightCount : ++whiteKnightCount;
                        break;
                    case ePieceType::KING:
                        color == ePieceColor::BLACK ? blackKing = true : whiteKing = true;
                        break;
                    default:
                        return false;
                    }
                }
            }

            if (!whiteKing || !blackKing)
            {
                return false;
            }

            const int whiteBishopsCount = whiteBishopLightCount + whiteBishopDarkCount;
            const int blackBishopsCount = blackBishopLightCount + blackBishopDarkCount;
            const int knightsCount      = whiteKnightCount + blackKnightCount;
            const int minorPiecesCount  = whiteBishopsCount + blackBishopsCount + knightsCount;

            if (minorPiecesCount <= 1)
            {
                return true;
            }

            if (minorPiecesCount == 2 && knightsCount == 0 && whiteBishopsCount == 1 && blackBishopsCount == 1)
            {
                const bool bishopsOnSameColor =
                    (whiteBishopLightCount == 1 && blackBishopLightCount == 1) || (whiteBishopDarkCount == 1 && blackBishopDarkCount == 1);
                return bishopsOnSameColor;
            }

            return false;
        }

        void RefreshMovesCountWithoutPawnAndTaking(const std::shared_ptr<Chessboard>& chessboard)
        {
            const auto piece = chessboard->GetPieceDirector()->GetPiece(chessboard->GetTo());
            if (!piece)
            {
                return;
            }

            const auto eatenPiecesCount = chessboard->GetPieceDirector()->GetEatenPieces().size();
            if (piece->GetColorAndType().type == ePieceType::PAWN || lastEatenPiecesCount_ != eatenPiecesCount)
            {
                halfMovesWithoutPawnMoveAndTaking_ = 0;
                lastEatenPiecesCount_              = eatenPiecesCount;
            }
            else
            {
                ++halfMovesWithoutPawnMoveAndTaking_;
            }
        }
    };
} // namespace Chess::Core
