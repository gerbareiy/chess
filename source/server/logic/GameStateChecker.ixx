module;
#include <memory>
export module Chess.GameStateChecker;
import Chess.Chessboard;
import Chess.DrawChecker;
import Chess.eGameState;
import Chess.MoveValidator;
import Chess.PieceDirector;

namespace Chess
{
    export class GameStateChecker
    {
        DrawChecker m_drawChecker;

    public:
        eGameState Calculate(const std::shared_ptr<Chessboard>& chessboard)
        {
            const bool hasNoMoves = chessboard->GetMoveValidator()->GetPiecesCanMoveCount() == 0;
            const bool isCheck    = chessboard->GetPieceDirector()->GetIsCheck();

            if (hasNoMoves && isCheck)
            {
                return eGameState::CHECKMATE;
            }
            if (m_drawChecker.IsDraw(chessboard))
            {
                return eGameState::DRAW;
            }
            if (isCheck)
            {
                return eGameState::CHECK;
            }
            return eGameState::PLAYING;
        }
    };
} // namespace Chess
