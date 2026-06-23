module;
#include <cstdlib>
#include <memory>
export module Chess.Pawn;
import Chess.Coordinate;
import Chess.ePieceColor;
import Chess.ePieceType;
import Chess.Piece;
import Chess.PieceColorAndType;
import Chess.Sizes;
import Chess.Utils.Exceptions;

namespace Chess
{
    export class Pawn final : public Piece
    {
        static constexpr int m_MAX_POSSIBLE_COUNT_MOVES = 2;

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

            const bool isOneStepForward  = fileDifference == 0 && rankDifference == moveVector;
            const bool isTwoStepsForward = fileDifference == 0 && m_isNotMoved && rankDifference == moveVector * m_MAX_POSSIBLE_COUNT_MOVES;
            const bool isDiagonalMove    = fileDifference == 1 && rankDifference == moveVector;
            if (!isOneStepForward && !isTwoStepsForward && !isDiagonalMove)
            {
                throw Utils::ImpossibleMoveException();
            }

            if (std::abs(rankDifference) == m_MAX_POSSIBLE_COUNT_MOVES)
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
    };
} // namespace Chess
