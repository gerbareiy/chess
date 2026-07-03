module;
#include "Piece.pb.h"
#include <memory>
export module Chess.Proto.Piece;
import Chess.Coordinate;
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
    export class Piece
    {
    public:
        static chess::proto::Piece ToProto(const std::shared_ptr<Chess::Piece>& piece)
        {
            chess::proto::Piece result;
            *result.mutable_color_and_type() = PieceColorAndType::ToProto(piece->GetColorAndType());
            *result.mutable_coordinate()     = Coordinate::ToProto(piece->GetPosition());

            if (const auto king = std::dynamic_pointer_cast<Chess::King>(piece))
            {
                result.set_king_can_castle(king->GetCanMakeCastling());
            }
            else if (const auto rook = std::dynamic_pointer_cast<Chess::Rook>(piece))
            {
                result.set_rook_can_castle(rook->GetCanMakeCastling());
            }
            else if (const auto pawn = std::dynamic_pointer_cast<Chess::Pawn>(piece))
            {
                result.set_pawn_can_en_passant(pawn->GetCanEnPassant());
                result.set_pawn_is_not_moved(pawn->GetIsNotMoved());
            }
            return result;
        }

        static std::shared_ptr<Chess::Piece> FromProto(const chess::proto::Piece& message)
        {
            const auto colorAndType = PieceColorAndType::FromProto(message.color_and_type());
            const auto coordinate   = Coordinate::FromProto(message.coordinate());

            if (colorAndType.type == Chess::ePieceType::KING)
            {
                return std::make_shared<Chess::King>(colorAndType.color, coordinate, message.king_can_castle());
            }
            return Chess::PieceFactory::Create(colorAndType, coordinate, nullptr);
        }
    };
} // namespace Chess::Proto
