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
import Chess.Utils.Exceptions;

namespace Chess
{
    export class King final : public ICastable, public Piece
    {
    public:
        King(ePieceColor color, const Coordinate& coordinate, bool canMakeCastling = true)
            : Piece(color, coordinate)
            , canMakeCastling_(canMakeCastling)
        {
        }

        virtual PieceColorAndType GetColorAndType() const override
        {
            return { GetColor(), ePieceType::KING };
        }

        virtual bool GetCanMakeCastling() const override
        {
            return canMakeCastling_;
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

                signalCastling_(to, side);
            }
            DisableCastling();
            Piece::Move(to);
        }

        bool GetIsCheck() const
        {
            return isCheck_;
        }

        void SetCheck(bool isCheck)
        {
            isCheck_ = isCheck;
        }

        std::optional<boost::signals2::connection> TryConnectCastling(const std::function<void(Coordinate, eCastleSide)>& subscriber)
        {
            return GetCanMakeCastling() ? std::make_optional(signalCastling_.connect(subscriber)) : std::nullopt;
        }

    private:
        bool canMakeCastling_ = false;
        bool isCheck_         = false;

        boost::signals2::signal<void(Coordinate, eCastleSide)> signalCastling_;

        void DisableCastling()
        {
            canMakeCastling_ = false;
            signalCastling_.disconnect_all_slots();
        }
    };
} // namespace Chess
