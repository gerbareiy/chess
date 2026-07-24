module;
#include "Chessboard.pb.h"
#include <cstdint>
#include <memory>
#include <vector>
export module Chess.Network.Chessboard;
import Chess.Core.ePieceColor;
import Chess.Core.Piece;
import Chess.Network.BoardReconstructor;
import Chess.Network.Piece;
import Chess.Network.PieceColorAndType;

namespace Chess::Network
{
    export class Chessboard
    {
    public:
        static chess::proto::Chessboard ToProto(
            const std::vector<std::shared_ptr<Chess::Core::Piece>>& pieces, Chess::Core::ePieceColor sideToMove, uint32_t ply = 0)
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

        static std::vector<std::shared_ptr<Chess::Core::Piece>> FromProto(const chess::proto::Chessboard& message)
        {
            return BoardReconstructor::Reconstruct(message);
        }
    };
} // namespace Chess::Network
