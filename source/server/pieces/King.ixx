module;
#include <boost/signals2.hpp>
export module Chess.King;
import Chess.Coordinate;
import Chess.eCastleSide;
import Chess.ePieceColor;
import Chess.ePieceType;
import Chess.ICastable;
import Chess.Piece;
import Chess.PieceColorAndType;
import Chess.Sizes;

namespace Chess
{
    export class King final : public ICastable, public Piece
    {
        bool m_canMakeCastling = true;
        bool m_isCheck         = false;

        boost::signals2::signal<void(Coordinate, eCastleSide)> m_signalCastling;

        void DisableCastling()
        {
            m_canMakeCastling = false;
        }

    public:
        King(ePieceColor color, const Coordinate& coordinate, bool canMakeCastling = true)
            : Piece(color, coordinate)
            , m_canMakeCastling(canMakeCastling)
        {
        }

        virtual PieceColorAndType GetColorAndType() const override
        {
            return { GetColor(), ePieceType::KING };
        }

        virtual bool GetCanMakeCastling() const override
        {
            return m_canMakeCastling;
        }

        virtual void Move(Coordinate to) override
        {
            DisableCastling();

            if (std::abs(GetPosition().file - to.file) > 1)
            {
                eCastleSide side;

                if (to.file > GetPosition().file)
                {
                    side = eCastleSide::RIGHT;
                }
                else if (to.file < GetPosition().file)
                {
                    side = eCastleSide::LEFT;
                }
                else
                {
                    throw std::logic_error("Move is impossible");
                }

                m_signalCastling(to, side);
            }
            Piece::Move(to);
        }

        bool GetIsCheck() const
        {
            return m_isCheck;
        }

        void SetCheck(bool isCheck)
        {
            m_isCheck = isCheck;
        }

        boost::signals2::connection ConnectCastling(const std::function<void(Coordinate, eCastleSide)>& subscriber)
        {
            return m_signalCastling.connect(subscriber);
        }
    };
} // namespace Chess
