module;
#include <stdexcept>
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

        void Init() const
        {
            if (m_playerColor == ePieceColor::NONE)
            {
                throw std::logic_error("Player color cannot be NONE");
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
