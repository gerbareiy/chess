module;
#include "Chessboard.pb.h"
#include <memory>
#include <vector>
export module Chess.Proto.BoardReconstructor;
import Chess.Coordinate;
import Chess.ePieceColor;
import Chess.ePieceType;
import Chess.King;
import Chess.Pawn;
import Chess.Piece;
import Chess.PieceColorAndType;
import Chess.PieceFactory;
import Chess.Proto.Coordinate;
import Chess.Proto.PieceColorAndType;
import Chess.Rook;

namespace Chess::Proto
{
    export class BoardReconstructor
    {
        static std::shared_ptr<King> KingOf(ePieceColor color, const std::shared_ptr<King>& whiteKing, const std::shared_ptr<King>& blackKing)
        {
            return color == ePieceColor::WHITE ? whiteKing : blackKing;
        }

    public:
        static std::vector<std::shared_ptr<Piece>> Reconstruct(const chess::proto::Chessboard& board)
        {
            std::vector<std::shared_ptr<Piece>> result;
            result.reserve(board.pieces_size());

            std::shared_ptr<King> whiteKing;
            std::shared_ptr<King> blackKing;

            for (const auto& piece : board.pieces())
            {
                const auto [color, type] = PieceColorAndType::FromProto(piece.color_and_type());
                if (type != ePieceType::KING)
                {
                    continue;
                }

                const auto coordinate = Coordinate::FromProto(piece.coordinate());
                auto       king       = std::make_shared<King>(color, coordinate, piece.king_can_castle());
                if (color == ePieceColor::WHITE)
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
                if (colorAndType.type == ePieceType::KING)
                {
                    continue;
                }

                const auto coordinate = Coordinate::FromProto(piece.coordinate());

                switch (colorAndType.type)
                {
                case ePieceType::ROOK:
                {
                    const auto king = piece.rook_can_castle() ? KingOf(colorAndType.color, whiteKing, blackKing) : nullptr;
                    result.push_back(std::make_shared<Rook>(colorAndType.color, coordinate, king));
                    break;
                }
                case ePieceType::PAWN:
                {
                    auto pawn = std::make_shared<Pawn>(colorAndType.color, coordinate);
                    pawn->RestoreState(piece.pawn_can_en_passant(), piece.pawn_is_not_moved());
                    result.push_back(std::move(pawn));
                    break;
                }
                default:
                    result.push_back(PieceFactory::Create(colorAndType, coordinate, nullptr));
                    break;
                }
            }

            return result;
        }
    };
} // namespace Chess::Proto
