module;
#include <memory>
#include <stdexcept>
export module Chess.MoveCheckerFactory;
import Chess.BishopChecker;
import Chess.eError;
import Chess.ePieceType;
import Chess.ErrorConverter;
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
        static std::shared_ptr<IMoveChecker> Create(const std::shared_ptr<Piece>& piece)
        {
            if (piece->GetColorAndType().type == ePieceType::BISHOP)
            {
                return std::make_shared<BishopChecker>();
            }
            if (piece->GetColorAndType().type == ePieceType::KING)
            {
                return std::make_shared<KingChecker>();
            }
            if (piece->GetColorAndType().type == ePieceType::KNIGHT)
            {
                return std::make_shared<KnightChecker>();
            }
            if (piece->GetColorAndType().type == ePieceType::PAWN)
            {
                return std::make_shared<PawnChecker>();
            }
            if (piece->GetColorAndType().type == ePieceType::QUEEN)
            {
                return std::make_shared<QueenChecker>();
            }
            if (piece->GetColorAndType().type == ePieceType::ROOK)
            {
                return std::make_shared<RookChecker>();
            }

            throw std::invalid_argument(ErrorConverter::ToString(eError::NOT_CORRECT_PIECE));
        }
    };
} // namespace Chess
