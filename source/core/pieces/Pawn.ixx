module;
#include <cstdlib>
#include <memory>
export module Chess.Core.Pawn;
import Chess.Constants.Counts;
import Chess.Core.Coordinate;
import Chess.Core.ePieceColor;
import Chess.Core.ePieceType;
import Chess.Core.Piece;
import Chess.Core.PieceColorAndType;
import Chess.Utils.Exceptions;

namespace Chess::Core
{
    export class Pawn final : public Piece
    {
    public:
        Pawn(ePieceColor color, const Coordinate& coordinate)
            : Piece(color, coordinate)
        {
            MakeTracking();
        }

        virtual PieceColorAndType GetColorAndType() const override
        {
            return { GetColor(), ePieceType::PAWN };
        }

        virtual void Move(Coordinate to) override
        {
            const int fileDifference = std::abs(to.file - GetPosition().file);
            const int rankDifference = to.rank - GetPosition().rank;
            const int moveVector     = GetColorAndType().color == ePieceColor::WHITE ? 1 : -1;

            const bool isOneStepForward = fileDifference == 0 && rankDifference == moveVector;
            const bool isTwoStepsForward =
                fileDifference == 0 && isNotMoved_ && rankDifference == moveVector * Constants::Counts::MAX_POSSIBLE_PAWN_MOVE_COUNT;
            const bool isDiagonalMove = fileDifference == 1 && rankDifference == moveVector;
            if (!isOneStepForward && !isTwoStepsForward && !isDiagonalMove)
            {
                throw Utils::ImpossibleMoveException();
            }

            if (std::abs(rankDifference) == Constants::Counts::MAX_POSSIBLE_PAWN_MOVE_COUNT)
            {
                canEnPassant_ = isNotMoved_;
            }
            else
            {
                LostEnPassant();
            }

            isOnPawnFirstMove_ = isNotMoved_;
            isNotMoved_        = false;
            Piece::Move(to);
        }

        bool GetCanEnPassant() const
        {
            return canEnPassant_;
        }

        bool GetIsNotMoved() const
        {
            return isNotMoved_;
        }

        void RestoreState(bool canEnPassant, bool isNotMoved)
        {
            canEnPassant_ = canEnPassant;
            isNotMoved_   = isNotMoved;
        }

    private:
        bool canEnPassant_      = false;
        bool isOnPawnFirstMove_ = false;
        bool isNotMoved_        = true;

        void LostEnPassant()
        {
            canEnPassant_ = false;
        }

        void MakeTracking()
        {
            if (!isOnPawnFirstMove_)
            {
                LostEnPassant();
            }
            isOnPawnFirstMove_ = false;
        }
    };
} // namespace Chess::Core
