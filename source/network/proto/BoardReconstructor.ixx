module;
#include "Chessboard.pb.h"
#include <memory>
#include <vector>
export module Chess.Network.BoardReconstructor;
import Chess.Core.Coordinate;
import Chess.Core.ePieceColor;
import Chess.Core.ePieceType;
import Chess.Core.King;
import Chess.Core.Pawn;
import Chess.Core.Piece;
import Chess.Core.PieceColorAndType;
import Chess.Core.PieceFactory;
import Chess.Core.Rook;
import Chess.Network.Coordinate;
import Chess.Network.PieceColorAndType;

namespace Chess::Network
{
    export class BoardReconstructor
    {
    public:
        static std::vector<std::shared_ptr<Chess::Core::Piece>> Reconstruct(const chess::proto::Chessboard& board)
        {
            std::vector<std::shared_ptr<Chess::Core::Piece>> result;
            result.reserve(board.pieces_size());

            std::shared_ptr<Chess::Core::King> whiteKing;
            std::shared_ptr<Chess::Core::King> blackKing;

            for (const auto& piece : board.pieces())
            {
                const auto [color, type] = PieceColorAndType::FromProto(piece.color_and_type());
                if (type != Chess::Core::ePieceType::KING)
                {
                    continue;
                }

                const auto coordinate = Coordinate::FromProto(piece.coordinate());
                auto       king       = std::make_shared<Chess::Core::King>(color, coordinate, piece.king_can_castle());
                if (color == Chess::Core::ePieceColor::WHITE)
                {
                    whiteKing = king;
                }
                else
                {
                    blackKing = king;
                }
                result.push_back(std::move(king));
            }

            for (const auto& piece : board.pieces())
            {
                const auto colorAndType = PieceColorAndType::FromProto(piece.color_and_type());
                if (colorAndType.type == Chess::Core::ePieceType::KING)
                {
                    continue;
                }

                const auto coordinate = Coordinate::FromProto(piece.coordinate());

                switch (colorAndType.type)
                {
                case Chess::Core::ePieceType::ROOK:
                {
                    const auto king = piece.rook_can_castle() ? KingOf(colorAndType.color, whiteKing, blackKing) : nullptr;
                    result.push_back(std::make_shared<Chess::Core::Rook>(colorAndType.color, coordinate, king));
                    break;
                }
                case Chess::Core::ePieceType::PAWN:
                {
                    auto pawn = std::make_shared<Chess::Core::Pawn>(colorAndType.color, coordinate);
                    pawn->RestoreState(piece.pawn_can_en_passant(), piece.pawn_is_not_moved());
                    result.push_back(std::move(pawn));
                    break;
                }
                default:
                    result.push_back(Chess::Core::PieceFactory::Create(colorAndType, coordinate, nullptr));
                    break;
                }
            }

            return result;
        }

    private:
        static std::shared_ptr<Chess::Core::King> KingOf(
            Chess::Core::ePieceColor color, const std::shared_ptr<Chess::Core::King>& whiteKing, const std::shared_ptr<Chess::Core::King>& blackKing)
        {
            return color == Chess::Core::ePieceColor::WHITE ? whiteKing : blackKing;
        }
    };
} // namespace Chess::Network
