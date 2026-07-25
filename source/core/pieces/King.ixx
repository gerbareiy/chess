module;
#include <cstdlib>
export module Chess.Core.King;
import Chess.Core.Coordinate;
import Chess.Core.eCastleSide;
import Chess.Core.ePieceColor;
import Chess.Core.ePieceType;
import Chess.Core.ICastable;
import Chess.Core.Piece;
import Chess.Core.PieceColorAndType;
import Chess.Utils.Event;
import Chess.Utils.Exceptions;

namespace Chess::Core
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

                onCastling.Invoke(to, side);
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

        template <typename Method, typename Class> bool TryTrackCastling(Method method, Class& subscriber)
        {
            if (!GetCanMakeCastling())
            {
                return false;
            }
            onCastling.Add(method, subscriber);
            return true;
        }

        template <typename Method, typename Class> void UntrackCastling(Method method, Class& subscriber)
        {
            onCastling.Remove(method, subscriber);
        }

    private:
        bool canMakeCastling_ = false;
        bool isCheck_         = false;

        Utils::Event<void(Coordinate, eCastleSide), King> onCastling;

        void DisableCastling()
        {
            canMakeCastling_ = false;
            onCastling.DisconnectAll();
        }
    };
} // namespace Chess::Core
