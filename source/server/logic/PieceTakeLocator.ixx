module;
#include <memory>
#include <vector>
export module Chess.PieceTakeLocator;
import Chess.Coordinate;
import Chess.CoordinateToPieceBuilder;
import Chess.Pawn;
import Chess.Piece;
import Chess.PieceFinder;

namespace Chess
{
    export class PieceTakeLocator
    {
    public:
        static Coordinate Find(const std::shared_ptr<Piece>& piece, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard, Coordinate to)
        {
            const auto pawnOnPassCoordinate = Coordinate(to.file, piece->GetPosition().rank);
            auto       pieceMap             = CoordinateToPieceBuilder::Build(piecesOnBoard);
            const auto finder               = std::make_shared<PieceFinder>(std::move(pieceMap));
            const auto currentPawn          = std::dynamic_pointer_cast<Pawn>(piece);
            const auto opponentPawn         = std::dynamic_pointer_cast<Pawn>(finder->Find(pawnOnPassCoordinate));

            if (currentPawn && opponentPawn && opponentPawn->GetCanEnPassant()
                && opponentPawn->GetColorAndType().color != currentPawn->GetColorAndType().color)
            {
                return pawnOnPassCoordinate;
            }
            return to;
        }
    };
} // namespace Chess
