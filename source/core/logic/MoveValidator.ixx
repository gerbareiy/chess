module;
#include "core/logic/Coordinate.h"

#include <memory>
#include <vector>
export module Chess.MoveValidator;
import Chess.Counts;
import Chess.MoveChecker;
import Chess.Piece;
import Chess.Player;

namespace Chess
{
    export class MoveValidator
    {
        std::vector<std::shared_ptr<Piece>> m_piecesCanMove;
        std::vector<Coordinate>             m_possibleMoves;
        std::vector<std::shared_ptr<Piece>> m_piecesOnBoard;
        std::shared_ptr<Player>             m_player;

    public:
        MoveValidator(const std::vector<std::shared_ptr<Piece>>& piecesOnBoard, const std::shared_ptr<Player>& player)
            : m_piecesOnBoard(piecesOnBoard)
            , m_player(player)
        {
            CalculatePiecesCanMove();
        }

        std::vector<Coordinate> GetPossibleMoves()
        {
            return m_possibleMoves;
        }

        void CalculatePiecesCanMove()
        {
            ClearPiecesCanMove();
            ClearPossibleMoves();

            std::vector<std::shared_ptr<Piece>> pieces;
            pieces.reserve(MAX_COUNT_ELEMENTS);

            for (const auto& piece : m_piecesOnBoard)
            {
                if (piece->GetColorAndType().color == m_player->GetPlayerColor())
                {
                    const auto moveChecker = MoveChecker(piece);

                    if (moveChecker.GetFilteredMoves(m_piecesOnBoard).size())
                    {
                        pieces.emplace_back(piece);
                    }
                }
            }

            m_piecesCanMove = pieces;
        }

        void   CalculatePossibleMoves(const std::shared_ptr<Piece>& piece);
        void   ClearPossibleMoves();
        void   ClearPiecesCanMove();
        size_t GetPiecesCanMoveCount();
        bool   IsCoordinateInPieceCanMove(const Coordinate& coordinate) const;
        bool   IsCoordinateInPossibleMoves(const Coordinate& coordinate) const;
        bool   IsValidMove(const std::shared_ptr<Piece>& piece, const Coordinate& to) const;
    };
} // namespace Chess
