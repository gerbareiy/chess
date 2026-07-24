module;
#include <memory>
#include <utility>
#include <vector>
export module Chess.Core.ChessboardFactory;
import Chess.Core.Chessboard;
import Chess.Core.ePieceColor;
import Chess.Core.MoveValidator;
import Chess.Core.Piece;
import Chess.Core.PieceDirector;
import Chess.Core.Player;

namespace Chess::Core
{
    export class ChessboardFactory
    {
    public:
        static std::shared_ptr<Chessboard> Create(std::vector<std::shared_ptr<Piece>> pieces, ePieceColor sideToMove)
        {
            const auto player    = std::make_shared<Player>(sideToMove);
            auto       director  = std::make_unique<PieceDirector>(pieces, player);
            auto       validator = std::make_unique<MoveValidator>(pieces, player);
            auto       board     = std::make_shared<Chessboard>(player, std::move(pieces), std::move(director), std::move(validator));
            board->Init();
            return board;
        }
    };
} // namespace Chess::Core
