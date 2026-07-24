module;
#include <memory>
#include <vector>
export module Chess.Core.Promotion;
import Chess.Constants.Sizes;
import Chess.Core.Bishop;
import Chess.Core.Coordinate;
import Chess.Core.ePieceColor;
import Chess.Core.ePieceType;
import Chess.Core.Knight;
import Chess.Core.Pawn;
import Chess.Core.Piece;
import Chess.Core.PieceFinder;
import Chess.Core.Promoter;
import Chess.Core.Queen;
import Chess.Core.Rook;

namespace Chess::Core
{
    export class Promotion
    {
    public:
        static void PromoteConditionally(
            const std::shared_ptr<Pawn>& pawn, std::vector<std::shared_ptr<Piece>>& piecesOnBoard, const std::shared_ptr<Promoter>& promoter)
        {
            if (!pawn)
            {
                return;
            }

            if (pawn->GetPosition().rank == 1 && pawn->GetColorAndType().color == ePieceColor::BLACK
                || pawn->GetPosition().rank == Constants::Sizes::CHESSBOARD_SIZE && pawn->GetColorAndType().color == ePieceColor::WHITE)
            {
                const auto             promoteType = promoter->GetPromoteType();
                std::shared_ptr<Piece> piece;
                auto                   color    = pawn->GetColorAndType().color;
                auto                   position = pawn->GetPosition();

                switch (promoteType)
                {
                case ePieceType::BISHOP:
                    piece = std::make_shared<Bishop>(color, position);
                    break;
                case ePieceType::KNIGHT:
                    piece = std::make_shared<Knight>(color, position);
                    break;
                case ePieceType::QUEEN:
                    piece = std::make_shared<Queen>(color, position);
                    break;
                case ePieceType::ROOK:
                    piece = std::make_shared<Rook>(color, position);
                    break;
                default:
                    break;
                }

                if (!piece)
                {
                    return;
                }
                piecesOnBoard.emplace_back(piece);
                piecesOnBoard.erase(std::ranges::find(piecesOnBoard, pawn));
            }
        }
    };
} // namespace Chess::Core
