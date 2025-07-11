module;
#include <memory>
#include <vector>
export module Chess.CheckChecker;
import Chess.ePieceColor;
import Chess.King;
import Chess.MoveCheckerFactory;
import Chess.Piece;

namespace Chess
{
    export class CheckChecker
    {
    public:
        static bool IsCheck(const std::shared_ptr<King>& king, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard)
        {
            for (const auto& piece : piecesOnBoard)
            {
                if (piece->GetColorAndType().color == king->GetColorAndType().color)
                {
                    continue;
                }

                const auto moveChecker = std::make_unique<MoveCheckerFactory>()->Create(piece);
                auto       moves       = moveChecker->GetMoves(piece, piecesOnBoard);

                for (const auto& move : moves)
                {
                    if (move == king->GetPosition())
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        static bool IsCheck(const ePieceColor& kingColor, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard)
        {
            for (const auto& piece : piecesOnBoard)
            {
                const auto king = std::dynamic_pointer_cast<King>(piece);

                if (king && king->GetColorAndType().color == kingColor)
                {
                    return IsCheck(king, piecesOnBoard);
                }
            }

            return false;
        }
    };
} // namespace Chess
