module;
#include <memory>
#include <stdexcept>
export module Chess.MoveCheckerFactory;
import Chess.BishopChecker;
import Chess.ePieceType;
import Chess.IMoveChecker;
import Chess.KingChecker;
import Chess.KnightChecker;
import Chess.PawnChecker;
import Chess.Piece;
import Chess.PieceColorAndType;
import Chess.QueenChecker;
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
                return std::make_unique<BishopChecker>();
            case ePieceType::KING:
                return std::make_unique<KingChecker>();
            case ePieceType::KNIGHT:
                return std::make_unique<KnightChecker>();
            case ePieceType::PAWN:
                return std::make_unique<PawnChecker>();
            case ePieceType::QUEEN:
                return std::make_unique<QueenChecker>();
            case ePieceType::ROOK:
                return std::make_unique<RookChecker>();
            case ePieceType::NONE:
                return nullptr;
            default:
                throw std::invalid_argument("Unknown piece type");
            }
        }
    };
} // namespace Chess
