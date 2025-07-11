module;
#include <functional>
#include <memory>
export module Chess.Player;
import Chess.ePieceColor;
import Chess.PieceSignalDirector;

namespace Chess
{
    export class Player
    {
    private:
        ePieceColor m_playerColor;

        void ChangeColor()
        {
            m_playerColor = m_playerColor == ePieceColor::BLACK ? ePieceColor::WHITE : ePieceColor::BLACK;
        }

    public:
        Player(ePieceColor firstMoveColor, const std::shared_ptr<PieceSignalDirector>& signalDirector)
            : m_playerColor(firstMoveColor == ePieceColor::NONE ? ePieceColor::WHITE : firstMoveColor)
        {
            signalDirector->ConnectMove(std::bind(&Player::ChangeColor, this));
        }

    public:
        ePieceColor GetPlayerColor()
        {
            return m_playerColor;
        }
    };
} // namespace Chess
