module;
#include <cstdint>
#include <memory>
#include <vector>
export module Chess.Net.BoardSnapshot;
import Chess.ePieceColor;
import Chess.Piece;

namespace Chess::Net
{
    export struct BoardSnapshot
    {
        std::vector<std::shared_ptr<Piece>> pieces;
        ePieceColor                         sideToMove = ePieceColor::NONE;
        uint32_t                            ply        = 0;
    };
} // namespace Chess::Net
