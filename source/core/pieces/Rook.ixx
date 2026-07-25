module;
#include <memory>
export module Chess.Core.Rook;
import Chess.Constants.Sizes;
import Chess.Core.Coordinate;
import Chess.Core.eCastleSide;
import Chess.Core.ePieceColor;
import Chess.Core.ePieceType;
import Chess.Core.ICastable;
import Chess.Core.King;
import Chess.Core.Piece;
import Chess.Core.PieceColorAndType;
import Chess.Utils.Exceptions;

namespace Chess::Core
{
    export class Rook final : public Piece, public ICastable
    {
    public:
        Rook(ePieceColor color, Coordinate coordinate, const std::shared_ptr<King>& king = nullptr)
            : Piece(color, coordinate)
        {
            TryMakeTracking(king);
        }

        ~Rook()
        {
            Untrack();
        }

        virtual PieceColorAndType GetColorAndType() const override
        {
            return { GetColor(), ePieceType::ROOK };
        }

        virtual bool GetCanMakeCastling() const override
        {
            if (!isTracking_)
            {
                return false;
            }
            const auto king = king_.lock();
            return king != nullptr && king->GetCanMakeCastling();
        }

        virtual void Move(Coordinate to) override
        {
            if (to.file != GetPosition().file && to.rank != GetPosition().rank)
            {
                throw Utils::ImpossibleMoveException();
            }
            if (to.file == GetPosition().file && to.rank == GetPosition().rank)
            {
                throw Utils::ImpossibleMoveException();
            }

            Untrack();
            Piece::Move(to);
        }

    private:
        std::weak_ptr<King> king_;
        bool                isTracking_ = false;

        void TryMakeTracking(const std::shared_ptr<King>& king)
        {
            if (king == nullptr || !king->GetCanMakeCastling())
            {
                return;
            }

            if (king->TryTrackCastling(&Rook::OnCastling, *this))
            {
                king_       = king;
                isTracking_ = true;
            }
        }

        void OnCastling(const Coordinate& to, eCastleSide side)
        {
            if (GetCanMakeCastling())
            {
                if (GetPosition().file == 'A' && side == eCastleSide::LEFT)
                {
                    Move(Coordinate(to.file + 1, to.rank));
                }
                else if (GetPosition().file == 'A' + Constants::Sizes::CHESSBOARD_SIZE - 1 && side == eCastleSide::RIGHT)
                {
                    Move(Coordinate(to.file - 1, to.rank));
                }
            }

            Untrack();
        }

        void Untrack()
        {
            if (!isTracking_)
            {
                return;
            }

            isTracking_ = false;
            if (const auto king = king_.lock())
            {
                king->UntrackCastling(&Rook::OnCastling, *this);
            }
        }
    };
} // namespace Chess::Core
