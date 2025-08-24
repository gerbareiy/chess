module;
#include <expected>
#include <memory>
#include <stdexcept>
export module Chess.Pawn;
import Chess.Coordinate;
import Chess.ePieceColor;
import Chess.ePieceType;
import Chess.Piece;
import Chess.PieceColorAndType;
import Chess.Sizes;

namespace Chess
{
    export class Pawn final : public Piece
    {
        static constexpr int m_MAX_POSSIBLE_COUNT_MOVES = 2;

        bool m_canEnPassant      = false;
        bool m_isOnPawnFirstMove = false;
        bool m_isNotMoved        = true;

        void LostEnPassant() { m_canEnPassant = false; }

        void MakeTracking()
        {
            if (!m_isOnPawnFirstMove)
            {
                LostEnPassant();
            }
            m_isOnPawnFirstMove = false;
        }

    public:
        Pawn(ePieceColor color, const Coordinate& coordinate)
            : Piece(PieceColorAndType(color, ePieceType::PAWN), coordinate)
        {
            MakeTracking();
        }

        bool GetCanEnPassant() const { return m_canEnPassant; }

        bool GetIsNotMoved() const { return m_isNotMoved; }

        virtual void Move(Coordinate to, bool isRealMove = true) override
        {
            if (isRealMove)
            {
                if (abs(to.rank - GetPosition().rank) == 2)
                {
                    m_canEnPassant = m_isNotMoved;
                }
                else
                {
                    LostEnPassant();
                }

                m_isOnPawnFirstMove = m_isNotMoved;
                m_isNotMoved        = false;
            }

            Piece::Move(to);
        }
    };
} // namespace Chess
