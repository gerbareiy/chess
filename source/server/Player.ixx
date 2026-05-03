export module Chess.Player;
import Chess.ePieceColor;

namespace Chess
{
    export class Player
    {
        ePieceColor m_playerColor;

    public:
        explicit Player(ePieceColor firstMoveColor)
            : m_playerColor(firstMoveColor)
        {
        }

        void Init()
        {
            if (m_playerColor == ePieceColor::NONE)
            {
                m_playerColor = ePieceColor::WHITE;
            }
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
