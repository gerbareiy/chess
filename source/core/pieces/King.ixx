module;
#include <boost/signals2.hpp>

#include <expected>
#include <memory>
export module Chess.King;
import Chess.Coordinate;
import Chess.eCastleSide;
import Chess.eError;
import Chess.ePieceColor;
import Chess.ePieceType;
import Chess.ErrorConverter;
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
        King(ePieceColor color, const Coordinate& coordinate)
            : Piece(PieceColorAndType(color, ePieceType::KING), coordinate)
        {
        }

        virtual bool GetCanMakeCastling() const override
        {
            return m_canMakeCastling;
        }

        bool GetIsCheck() const
        {
            return m_isCheck;
        }

        void SetCheck(bool isCheck)
        {
            m_isCheck = isCheck;
        }

        virtual void Move(Coordinate to, bool isRealMove = true) override
        {
            if (isRealMove)
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
                        throw std::logic_error(ErrorConverter::ToString(eError::NOT_CORRECT_MOVE));
                    }

                    m_signalCastling(to, side);
                }
            }

            Piece::Move(to);
        }

        boost::signals2::connection ConnectCastling(const boost::signals2::signal<void(Coordinate, eCastleSide)>::slot_type& subscriber)
        {
            return m_signalCastling.connect(subscriber);
        }
    };
} // namespace Chess
