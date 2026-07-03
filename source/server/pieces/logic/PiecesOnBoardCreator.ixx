module;
#include <memory>
#include <stdexcept>
#include <vector>
export module Chess.PiecesOnBoardCreator;
import Chess.Coordinate;
import Chess.CoordinateToPieceFactory;
import Chess.ePieceColor;
import Chess.ePieceType;
import Chess.ICastable;
import Chess.King;
import Chess.Piece;
import Chess.PieceColorAndType;
import Chess.PieceFinder;
import Chess.PieceFactory;
import Chess.PiecePlacement;

namespace Chess
{
    export class PiecesOnBoardCreator
    {
    public:
        static std::vector<std::shared_ptr<Piece>> Create(
            const std::shared_ptr<King>& whiteKing, const std::shared_ptr<King>& blackKing, const std::vector<PiecePlacement>& nonKingPieces)
        {
            std::vector<std::shared_ptr<Piece>> piecesOnBoard;
            piecesOnBoard.reserve(nonKingPieces.size() + 2);

            piecesOnBoard.push_back(whiteKing);
            piecesOnBoard.push_back(blackKing);

            for (const auto& [colorAndType, coordinate] : nonKingPieces)
            {
                const auto& king = colorAndType.color == ePieceColor::WHITE ? whiteKing : blackKing;
                piecesOnBoard.push_back(PieceFactory::Create(colorAndType, coordinate, king));
            }

            return piecesOnBoard;
        }

        static std::vector<std::shared_ptr<Piece>> Create(const std::vector<std::shared_ptr<Piece>>& piecesOnBoard)
        {
            auto       pieceMap = CoordinateToPieceFactory::Create(piecesOnBoard);
            const auto finder   = std::make_shared<PieceFinder>(std::move(pieceMap));

            constexpr auto whiteKingColorAndType = PieceColorAndType{ ePieceColor::WHITE, ePieceType::KING };
            constexpr auto blackKingColorAndType = PieceColorAndType{ ePieceColor::BLACK, ePieceType::KING };

            const auto whiteKingPosition = finder->TryFind(whiteKingColorAndType).value();
            const auto blackKingPosition = finder->TryFind(blackKingColorAndType).value();

            const auto originalWhiteKing = std::dynamic_pointer_cast<ICastable>(finder->TryFind(whiteKingPosition));
            const auto originalBlackKing = std::dynamic_pointer_cast<ICastable>(finder->TryFind(blackKingPosition));
            if (!originalWhiteKing || !originalBlackKing)
            {
                throw std::runtime_error("Kings not found on the board");
            }

            const auto whiteKing = std::make_shared<King>(whiteKingColorAndType.color, whiteKingPosition, originalWhiteKing->GetCanMakeCastling());
            const auto blackKing = std::make_shared<King>(blackKingColorAndType.color, blackKingPosition, originalBlackKing->GetCanMakeCastling());

            std::vector<PiecePlacement> nonKingPieces;
            nonKingPieces.reserve(piecesOnBoard.size());
            for (const auto& piece : piecesOnBoard)
            {
                if (piece->GetColorAndType().type != ePieceType::KING)
                {
                    nonKingPieces.push_back({ piece->GetColorAndType(), piece->GetPosition() });
                }
            }

            return Create(whiteKing, blackKing, nonKingPieces);
        }
    };

} // namespace Chess
