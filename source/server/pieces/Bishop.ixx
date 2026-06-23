module;
#include <cstdlib>
export module Chess.Bishop;
import Chess.Coordinate;
import Chess.ePieceColor;
import Chess.ePieceType;
import Chess.Piece;
import Chess.PieceColorAndType;
import Chess.Sizes;
import Chess.Utils.Exceptions;

namespace Chess
{
    export class Bishop final : public Piece
    {
    public:
        Bishop(ePieceColor color, const Coordinate& coordinate)
            : Piece(color, coordinate)
        {
        }

        virtual PieceColorAndType GetColorAndType() const override
        {
            return { GetColor(), ePieceType::BISHOP };
        }

        virtual void Move(Coordinate to) override
        {
            const int fileDifference = std::abs(to.file - GetPosition().file);
            const int rankDifference = std::abs(to.rank - GetPosition().rank);
            if (fileDifference == 0 || fileDifference != rankDifference)
            {
                throw Utils::ImpossibleMoveException();
            }
            Piece::Move(to);
        }
    };
} // namespace Chess
