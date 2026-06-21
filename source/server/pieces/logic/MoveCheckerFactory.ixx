module;
#include <memory>
#include <stdexcept>
export module Chess.MoveCheckerFactory;
import Chess.Bishop;
import Chess.BishopChecker;
import Chess.ePieceType;
import Chess.IMoveChecker;
import Chess.King;
import Chess.KingChecker;
import Chess.Knight;
import Chess.KnightChecker;
import Chess.Pawn;
import Chess.PawnChecker;
import Chess.Piece;
import Chess.PieceColorAndType;
import Chess.Queen;
import Chess.QueenChecker;
import Chess.Rook;
import Chess.RookChecker;

namespace Chess
{
    export class MoveCheckerFactory
    {
    public:
        static std::unique_ptr<IMoveChecker> Create(const std::shared_ptr<Piece>& piece)
        {
            switch (piece->GetColorAndType().type)
            {
            case ePieceType::BISHOP:
                return std::make_unique<BishopChecker>(std::dynamic_pointer_cast<Bishop>(piece));
            case ePieceType::KING:
                return std::make_unique<KingChecker>(std::dynamic_pointer_cast<King>(piece));
            case ePieceType::KNIGHT:
                return std::make_unique<KnightChecker>(std::dynamic_pointer_cast<Knight>(piece));
            case ePieceType::PAWN:
                return std::make_unique<PawnChecker>(std::dynamic_pointer_cast<Pawn>(piece));
            case ePieceType::QUEEN:
                return std::make_unique<QueenChecker>(std::dynamic_pointer_cast<Queen>(piece));
            case ePieceType::ROOK:
                return std::make_unique<RookChecker>(std::dynamic_pointer_cast<Rook>(piece));
            case ePieceType::NONE:
                throw std::invalid_argument("Piece type cannot be NONE");
            default:
                throw std::invalid_argument("Unknown piece type");
            }
        }
    };
} // namespace Chess
