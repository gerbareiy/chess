module;
#include "Chessboard.pb.h"
#include <cstdint>
#include <memory>
#include <vector>
export module Chess.Proto.Chessboard;
import Chess.ePieceColor;
import Chess.Piece;
import Chess.Proto.BoardReconstructor;
import Chess.Proto.PieceColorAndType;
import Chess.Proto.Piece;

namespace Chess::Proto
{
    export class Chessboard
    {
    public:
        static chess::proto::Chessboard ToProto(const std::vector<std::shared_ptr<Chess::Piece>>& pieces, Chess::ePieceColor sideToMove, uint32_t ply = 0)
        {
            chess::proto::Chessboard result;
            for (const auto& piece : pieces)
            {
                *result.add_pieces() = Piece::ToProto(piece);
            }
            result.set_side_to_move(PieceColorAndType::ToProto(sideToMove));
            result.set_ply(ply);
            return result;
        }

        static std::vector<std::shared_ptr<Chess::Piece>> FromProto(const chess::proto::Chessboard& message)
        {
            return BoardReconstructor::Reconstruct(message);
        }
    };
} // namespace Chess::Proto
