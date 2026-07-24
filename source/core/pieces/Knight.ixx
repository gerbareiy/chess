module;
#include <cstdlib>
export module Chess.Core.Knight;
import Chess.Core.Coordinate;
import Chess.Core.ePieceColor;
import Chess.Core.ePieceType;
import Chess.Core.Piece;
import Chess.Core.PieceColorAndType;
import Chess.Utils.Exceptions;

namespace Chess::Core
{
    export class Knight final : public Piece
    {
    public:
        Knight(ePieceColor color, const Coordinate& coordinate)
            : Piece(color, coordinate)
        {
        }

        virtual PieceColorAndType GetColorAndType() const override
        {
            return { GetColor(), ePieceType::KNIGHT };
        }

        virtual void Move(Coordinate to) override
        {
            const int fileDifference = std::abs(to.file - GetPosition().file);
            const int rankDifference = std::abs(to.rank - GetPosition().rank);
            if (!((fileDifference == 1 && rankDifference == 2) || (fileDifference == 2 && rankDifference == 1)))
            {
                throw Utils::ImpossibleMoveException();
            }
            Piece::Move(to);
        }
    };
} // namespace Chess::Core
