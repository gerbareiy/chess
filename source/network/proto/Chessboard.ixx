module;
#include "Chessboard.pb.h"

#include <cstdint>
#include <memory>
#include <vector>
export module Chess.Proto.Chessboard;
import Chess.ePieceColor;
import Chess.Piece;
import Chess.Proto.PieceColorAndType;
import Chess.Proto.Piece;

namespace Chess::Proto
{
    export class Chessboard
    {
    public:
        static chess::proto::Chessboard ToProto(
            const std::vector<std::shared_ptr<Chess::Piece>>& pieces, Chess::ePieceColor sideToMove, uint32_t halfMoveClock)
        {
            chess::proto::Chessboard result;
            for (const auto& piece : pieces)
            {
                *result.add_pieces() = Piece::ToProto(piece);
            }
            result.set_side_to_move(PieceColorAndType::ToProto(sideToMove));
            result.set_half_move_clock(halfMoveClock);
            return result;
        }

        static std::vector<std::shared_ptr<Chess::Piece>> FromProto(const chess::proto::Chessboard& message)
        {
            std::vector<std::shared_ptr<Chess::Piece>> result;
            result.reserve(message.pieces_size());
            for (const auto& piece : message.pieces())
            {
                result.push_back(Piece::FromProto(piece));
            }
            return result;
        }
    };
} // namespace Chess::Proto
