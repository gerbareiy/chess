module;
#include <memory>
#include <ranges>
#include <stdexcept>
#include <vector>
export module Chess.PiecesOnBoardCreator;
import Chess.Piece;
import Chess.CoordinateToPieceFactory;
import Chess.ePieceColor;
import Chess.ePieceType;
import Chess.ICastable;
import Chess.King;
import Chess.PieceFinder;
import Chess.PieceFactory;
import Chess.PieceColorAndType;

namespace Chess
{
    export class PiecesOnBoardCreator
    {
    public:
        static std::vector<std::shared_ptr<Piece>> Create(const std::vector<std::shared_ptr<Piece>>& piecesOnBoard)
        {
            auto       pieceMap = CoordinateToPieceFactory::Create(piecesOnBoard);
            const auto finder   = std::make_shared<PieceFinder>(std::move(pieceMap));

            std::vector<std::shared_ptr<Piece>> piecesOnBoardCopy;
            piecesOnBoardCopy.reserve(piecesOnBoard.size());

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

            piecesOnBoardCopy.push_back(whiteKing);
            piecesOnBoardCopy.push_back(blackKing);

            const auto filtered = [&piecesOnBoard = std::as_const(piecesOnBoard)](ePieceColor color)
            {
                return piecesOnBoard
                       | std::views::filter([color](const auto& piece)
                                            { return piece->GetColorAndType().color == color && piece->GetColorAndType().type != ePieceType::KING; });
            };

            for (const auto& piece : filtered(ePieceColor::WHITE))
            {
                piecesOnBoardCopy.push_back(PieceFactory::Create(piece->GetColorAndType(), piece->GetPosition(), whiteKing));
            }
            for (const auto& piece : filtered(ePieceColor::BLACK))
            {
                piecesOnBoardCopy.push_back(PieceFactory::Create(piece->GetColorAndType(), piece->GetPosition(), blackKing));
            }
            return piecesOnBoardCopy;
        }
    };

} // namespace Chess
