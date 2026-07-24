module;
#include <cstdint>
#include <memory>
#include <vector>
export module Chess.Network.BoardSnapshot;
import Chess.Core.ePieceColor;
import Chess.Core.Piece;

namespace Chess::Network
{
    export struct BoardSnapshot
    {
        std::vector<std::shared_ptr<Core::Piece>> pieces;
        Core::ePieceColor                         sideToMove = Core::ePieceColor::NONE;
        uint32_t                                  ply        = 0;
    };
} // namespace Chess::Network
