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
        bool m_canMakeCastling = false;
        bool m_isCheck         = false;

        boost::signals2::signal<void(Coordinate, eCastleSide)> m_signalCastling;

        void DisableCastling()
        {
            m_canMakeCastling = false;
            m_signalCastling.disconnect_all_slots();
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
            if (std::abs(GetPosition().file - to.file) > 2)
            {
                throw std::logic_error("Impossible move");
            }
            if (std::abs(GetPosition().rank - to.rank) > 1)
            {
                throw std::logic_error("Impossible move");
            }

            if (GetCanMakeCastling() && std::abs(GetPosition().file - to.file) == 2)
            {
                if (to.rank != GetPosition().rank)
                {
                    throw std::logic_error("Impossible move");
                }

                auto side = eCastleSide::RIGHT;
                if (to.file > GetPosition().file)
                {
                }
                else if (to.file < GetPosition().file)
                {
                    side = eCastleSide::LEFT;
                }
                else
                {
                    throw std::logic_error("Impossible move");
                }

                m_signalCastling(to, side);
            }
            DisableCastling();
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

        std::optional<boost::signals2::connection> TryConnectCastling(const std::function<void(Coordinate, eCastleSide)>& subscriber)
        {
            if (GetCanMakeCastling())
            {
                return m_signalCastling.connect(subscriber);
            }
            return std::nullopt;
        }
    };
} // namespace Chess
