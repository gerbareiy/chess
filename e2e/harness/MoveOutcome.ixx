export module Chess.E2E.MoveOutcome;

namespace Chess::E2E
{
    export enum class MoveOutcome
    {
        Ongoing,
        IllegalMove,
        NoMovablePiece,
        Checkmate,
        Draw
    };
} // namespace Chess::E2E
