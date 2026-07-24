module;
#include "Piece.pb.h"
#include <memory>
export module Chess.Network.Piece;
import Chess.Core.Coordinate;
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
    export class Piece
    {
    public:
        static chess::proto::Piece ToProto(const std::shared_ptr<Core::Piece>& piece)
        {
            chess::proto::Piece result;
            *result.mutable_color_and_type() = PieceColorAndType::ToProto(piece->GetColorAndType());
            *result.mutable_coordinate()     = Coordinate::ToProto(piece->GetPosition());

            if (const auto king = std::dynamic_pointer_cast<Core::King>(piece))
            {
                result.set_king_can_castle(king->GetCanMakeCastling());
            }
            else if (const auto rook = std::dynamic_pointer_cast<Core::Rook>(piece))
            {
                result.set_rook_can_castle(rook->GetCanMakeCastling());
            }
            else if (const auto pawn = std::dynamic_pointer_cast<Core::Pawn>(piece))
            {
                result.set_pawn_can_en_passant(pawn->GetCanEnPassant());
                result.set_pawn_is_not_moved(pawn->GetIsNotMoved());
            }
            return result;
        }

        static std::shared_ptr<Core::Piece> FromProto(const chess::proto::Piece& message)
        {
            const auto colorAndType = PieceColorAndType::FromProto(message.color_and_type());
            const auto coordinate   = Coordinate::FromProto(message.coordinate());

            if (colorAndType.type == Core::ePieceType::KING)
            {
                return std::make_shared<Core::King>(colorAndType.color, coordinate, message.king_can_castle());
            }
            return Core::PieceFactory::Create(colorAndType, coordinate, nullptr);
        }
    };
} // namespace Chess::Network
