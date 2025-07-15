module;
#include <memory>
#include <vector>
export module Chess.Promotion;
import Chess.Bishop;
import Chess.Coordinate;
import Chess.ePieceColor;
import Chess.ePieceType;
import Chess.Knight;
import Chess.Pawn;
import Chess.Piece;
import Chess.PieceFinder;
import Chess.Promoter;
import Chess.Queen;
import Chess.Rook;
import Chess.Sizes;

namespace Chess
{
    export class Promotion
    {
    public:
        static void PromoteConditionally(const std::shared_ptr<Pawn>&         pawn,
                                         std::vector<std::shared_ptr<Piece>>& piecesOnBoard,
                                         const std::shared_ptr<Promoter>&     promoter)
        {
            if (!pawn)
            {
                return;
            }

            if (pawn->GetPosition().rank == 1 && pawn->GetColorAndType().color == ePieceColor::BLACK
                || pawn->GetPosition().rank == CHESSBOARD_SIZE && pawn->GetColorAndType().color == ePieceColor::WHITE)
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
} // namespace Chess
