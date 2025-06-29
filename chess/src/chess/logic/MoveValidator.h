#pragma once

#include <memory>
#include <vector>

namespace Chess
{
    class Piece;
    class CheckChecker;
    class MoveChecker;
    class Player;
    struct Coordinate;

    class MoveValidator
    {
        std::vector<std::shared_ptr<Piece>>        m_piecesCanMove;
        std::vector<Coordinate>                    m_possibleMoves;
        const std::vector<std::shared_ptr<Piece>>& m_piecesOnBoard;
        const std::shared_ptr<Player>              m_player;

    public:
        MoveValidator(const std::vector<std::shared_ptr<Piece>>& piecesOnBoard, const std::shared_ptr<Player>& player);

        std::vector<Coordinate> GetPossibleMoves();

        void CalculatePiecesCanMove();
        void CalculatePossibleMoves(const std::shared_ptr<Piece>& piece);
        void ClearPossibleMoves();
        void ClearPiecesCanMove();
        size_t  GetPiecesCanMoveCount();
        bool IsCoordinateInPieceCanMove(const Coordinate& coordinate) const;
        bool IsCoordinateInPossibleMoves(const Coordinate& coordinate) const;
        bool IsValidMove(const std::shared_ptr<Piece>& piece, const Coordinate& to) const;
    };
} // namespace Chess
