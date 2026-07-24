module;
#include <memory>
#include <stdexcept>
export module Chess.Core.MoveCheckerFactory;
import Chess.Core.Bishop;
import Chess.Core.BishopChecker;
import Chess.Core.ePieceType;
import Chess.Core.IMoveChecker;
import Chess.Core.King;
import Chess.Core.KingChecker;
import Chess.Core.Knight;
import Chess.Core.KnightChecker;
import Chess.Core.Pawn;
import Chess.Core.PawnChecker;
import Chess.Core.Piece;
import Chess.Core.PieceColorAndType;
import Chess.Core.Queen;
import Chess.Core.QueenChecker;
import Chess.Core.Rook;
import Chess.Core.RookChecker;

namespace Chess::Core
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
} // namespace Chess::Core
