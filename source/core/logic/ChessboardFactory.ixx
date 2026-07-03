module;
#include <memory>
#include <utility>
#include <vector>
export module Chess.ChessboardFactory;
import Chess.Chessboard;
import Chess.ePieceColor;
import Chess.MoveValidator;
import Chess.Piece;
import Chess.PieceDirector;
import Chess.Player;

namespace Chess
{
    // Собирает готовую к игре доску из набора фигур и цвета, чей ход. Используется сервером
    // (стартовая расстановка) и клиентом (перестройка доски из присланного состояния).
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
} // namespace Chess
