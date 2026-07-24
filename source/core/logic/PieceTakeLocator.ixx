module;
#include <memory>
#include <vector>
export module Chess.Core.PieceTakeLocator;
import Chess.Core.Coordinate;
import Chess.Core.CoordinateToPieceFactory;
import Chess.Core.Pawn;
import Chess.Core.Piece;
import Chess.Core.PieceFinder;

namespace Chess::Core
{
    export class PieceTakeLocator
    {
    public:
        static Coordinate Find(const std::shared_ptr<Piece>& piece, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard, Coordinate to)
        {
            const auto pawnOnPassCoordinate = Coordinate(to.file, piece->GetPosition().rank);
            auto       pieceMap             = CoordinateToPieceFactory::Create(piecesOnBoard);
            const auto finder               = std::make_shared<PieceFinder>(std::move(pieceMap));
            const auto currentPawn          = std::dynamic_pointer_cast<Pawn>(piece);
            const auto opponentPawn         = std::dynamic_pointer_cast<Pawn>(finder->TryFind(pawnOnPassCoordinate));

            if (currentPawn && opponentPawn && opponentPawn->GetCanEnPassant()
                && opponentPawn->GetColorAndType().color != currentPawn->GetColorAndType().color)
            {
                return pawnOnPassCoordinate;
            }
            return to;
        }
    };
} // namespace Chess::Core
