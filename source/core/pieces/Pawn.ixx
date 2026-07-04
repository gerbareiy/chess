module;
#include <cstdlib>
#include <memory>
export module Chess.Pawn;
import Chess.Constants.Counts;
import Chess.Coordinate;
import Chess.ePieceColor;
import Chess.ePieceType;
import Chess.Piece;
import Chess.PieceColorAndType;
import Chess.Utils.Exceptions;

namespace Chess
{
    export class Pawn final : public Piece
    {
        bool m_canEnPassant      = false;
        bool m_isOnPawnFirstMove = false;
        bool m_isNotMoved        = true;

        void LostEnPassant()
        {
            m_canEnPassant = false;
        }

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
                fileDifference == 0 && m_isNotMoved && rankDifference == moveVector * Constants::Counts::MAX_POSSIBLE_PAWN_MOVE_COUNT;
            const bool isDiagonalMove = fileDifference == 1 && rankDifference == moveVector;
            if (!isOneStepForward && !isTwoStepsForward && !isDiagonalMove)
            {
                throw Utils::ImpossibleMoveException();
            }

            if (std::abs(rankDifference) == Constants::Counts::MAX_POSSIBLE_PAWN_MOVE_COUNT)
            {
                m_canEnPassant = m_isNotMoved;
            }
            else
            {
                LostEnPassant();
            }

            m_isOnPawnFirstMove = m_isNotMoved;
            m_isNotMoved        = false;
            Piece::Move(to);
        }

        bool GetCanEnPassant() const
        {
            return m_canEnPassant;
        }

        bool GetIsNotMoved() const
        {
            return m_isNotMoved;
        }

        void RestoreState(bool canEnPassant, bool isNotMoved)
        {
            m_canEnPassant = canEnPassant;
            m_isNotMoved   = isNotMoved;
        }
    };
} // namespace Chess
