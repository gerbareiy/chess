module;
#include <cstdlib>
export module Chess.Core.Queen;
import Chess.Core.Coordinate;
import Chess.Core.ePieceColor;
import Chess.Core.ePieceType;
import Chess.Core.Piece;
import Chess.Core.PieceColorAndType;
import Chess.Utils.Exceptions;

namespace Chess::Core
{
    export class Queen final : public Piece
    {
    public:
        Queen(ePieceColor color, const Coordinate& coordinate)
            : Piece(color, coordinate)
        {
        }

        virtual PieceColorAndType GetColorAndType() const override
        {
            return { GetColor(), ePieceType::QUEEN };
        }

        virtual void Move(Coordinate to) override
        {
            const int fileDifference = std::abs(to.file - GetPosition().file);
            const int rankDifference = std::abs(to.rank - GetPosition().rank);

            const bool isStraightMove = fileDifference == 0 || rankDifference == 0;
            const bool isDiagonalMove = fileDifference == rankDifference;
            if ((fileDifference == 0 && rankDifference == 0) || (!isStraightMove && !isDiagonalMove))
            {
                throw Utils::ImpossibleMoveException();
            }

            Piece::Move(to);
        }
    };
} // namespace Chess::Core
