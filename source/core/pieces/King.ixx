module;
#include <boost/signals2.hpp>
#include <cstdlib>
export module Chess.King;
import Chess.Coordinate;
import Chess.eCastleSide;
import Chess.ePieceColor;
import Chess.ePieceType;
import Chess.ICastable;
import Chess.Piece;
import Chess.PieceColorAndType;
import Chess.Sizes;
import Chess.Utils.Exceptions;

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
            const int fileDifference = std::abs(GetPosition().file - to.file);
            const int rankDifference = std::abs(GetPosition().rank - to.rank);

            if (fileDifference > 2)
            {
                throw Utils::ImpossibleMoveException();
            }
            if (rankDifference > 1)
            {
                throw Utils::ImpossibleMoveException();
            }
            if (fileDifference == 0 && rankDifference == 0)
            {
                throw Utils::ImpossibleMoveException();
            }

            if (fileDifference == 2)
            {
                if (!GetCanMakeCastling())
                {
                    throw Utils::ImpossibleMoveException();
                }
                if (to.rank != GetPosition().rank)
                {
                    throw Utils::ImpossibleMoveException();
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
                    throw Utils::ImpossibleMoveException();
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
