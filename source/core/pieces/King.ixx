module;
#include <boost/signals2.hpp>

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
import Chess.PieceSignalDirector;
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
        void MakeTracking(const std::shared_ptr<PieceSignalDirector>& signalDirector)
        {
            if (!signalDirector)
            {
                return;
            }

            signalDirector->ConnectMoveWithCheck(
                [this](bool isCheck)
                {
                    m_isCheck = isCheck;
                });
        }

    public:
        King(ePieceColor color)
        {
            m_colorAndType = PieceColorAndType(color, ePieceType::KING);

            switch (color)
            {
            case ePieceColor::BLACK:
                m_position = Coordinate('E', CHESSBOARD_SIZE);
                break;
            case ePieceColor::WHITE:
                m_position = Coordinate('E', 1);
                break;
            default:
                throw std::out_of_range(ErrorConverter::ToString(eError::OUT_OF_CHESSBOARD));
            }
        }

        King(ePieceColor color, const std::shared_ptr<PieceSignalDirector>& signalDirector)
            : King(color)
        {
            MakeTracking(signalDirector);
        }

        King(ePieceColor color, const Coordinate& coordinate)
            : Piece(PieceColorAndType(color, ePieceType::KING), coordinate)
        {
        }

        King(ePieceColor color, const Coordinate& coordinate, const std::shared_ptr<PieceSignalDirector>& signalDirector)
            : King(color, coordinate)
        {
            MakeTracking(signalDirector);
        }

        virtual bool GetCanMakeCastling() const override
        {
            return m_canMakeCastling;
        }

        bool GetIsCheck() const
        {
            return m_isCheck;
        }

        virtual void Move(Coordinate to, bool isRealMove = true) override
        {
            if (isRealMove)
            {
                DisableCastling();

                if (abs(GetPosition().file - to.file) > 1)
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
                        throw std::invalid_argument(ErrorConverter::ToString(eError::NOT_CORRECT_MOVE));
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
