module;
#include <memory>
export module Chess.Core.GameStateChecker;
import Chess.Core.Chessboard;
import Chess.Core.DrawChecker;
import Chess.Core.eGameState;
import Chess.Core.MoveValidator;
import Chess.Core.PieceDirector;

namespace Chess::Core
{
    export class GameStateChecker
    {
    public:
        eGameState Calculate(const std::shared_ptr<Chessboard>& chessboard)
        {
            const bool hasNoMoves = chessboard->GetMoveValidator()->GetPiecesCanMoveCount() == 0;
            const bool isCheck    = chessboard->GetPieceDirector()->GetIsCheck();

            if (hasNoMoves && isCheck)
            {
                return eGameState::CHECKMATE;
            }
            if (drawChecker_.IsDraw(chessboard))
            {
                return eGameState::DRAW;
            }
            if (isCheck)
            {
                return eGameState::CHECK;
            }
            return eGameState::PLAYING;
        }

    private:
        DrawChecker drawChecker_;
    };
} // namespace Chess::Core
