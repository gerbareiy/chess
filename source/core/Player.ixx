export module Chess.Core.Player;
import Chess.Core.ePieceColor;

namespace Chess::Core
{
    export class Player
    {
    public:
        explicit Player(ePieceColor firstMoveColor)
            : playerColor_(firstMoveColor)
        {
        }

        void Init()
        {
            if (playerColor_ == ePieceColor::NONE)
            {
                playerColor_ = ePieceColor::WHITE;
            }
        }

        void InverseColor()
        {
            playerColor_ = playerColor_ == ePieceColor::BLACK ? ePieceColor::WHITE : ePieceColor::BLACK;
        }

        ePieceColor GetPlayerColor() const
        {
            return playerColor_;
        }

    private:
        ePieceColor playerColor_;
    };
} // namespace Chess::Core
