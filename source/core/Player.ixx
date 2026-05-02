module;
#include <functional>
#include <memory>
export module Chess.Player;
import Chess.ePieceColor;

namespace Chess
{
    export class Player
    {
        ePieceColor m_playerColor;

    public:
        Player(ePieceColor firstMoveColor)
        {
            m_playerColor = firstMoveColor == ePieceColor::NONE ? ePieceColor::WHITE : firstMoveColor;
        }

        void ChangeColor()
        {
            m_playerColor = m_playerColor == ePieceColor::BLACK ? ePieceColor::WHITE : ePieceColor::BLACK;
        }

        ePieceColor GetPlayerColor()
        {
            return m_playerColor;
        }
    };
} // namespace Chess
