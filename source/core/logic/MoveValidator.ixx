module;
#include <memory>
#include <vector>
export module Chess.MoveValidator;
import Chess.Coordinate;
import Chess.Counts;
import Chess.MoveCheckerOwner;
import Chess.Piece;
import Chess.PieceFinder;
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
                    const auto moveChecker = MoveCheckerOwner(piece);

                    if (moveChecker.GetFilteredMoves(m_piecesOnBoard).size())
                    {
                        pieces.emplace_back(piece);
                    }
                }
            }

            m_piecesCanMove = pieces;
        }

        void CalculatePossibleMoves(const std::shared_ptr<Piece>& piece)
        {
            const auto it = std::ranges::find(m_piecesCanMove, piece);

            if (it != m_piecesCanMove.end())
            {
                const auto moveChecker = std::make_shared<MoveCheckerOwner>(piece);
                m_possibleMoves        = moveChecker->GetFilteredMoves(m_piecesOnBoard);
            }
        }

        void ClearPossibleMoves()
        {
            m_possibleMoves.clear();
        }

        void ClearPiecesCanMove()
        {
            m_piecesCanMove.clear();
        }

        size_t GetPiecesCanMoveCount()
        {
            return m_piecesCanMove.size();
        }

        bool IsCoordinateInPieceCanMove(const Coordinate& coordinate) const
        {
            const auto finder = std::make_unique<PieceFinder>(m_piecesCanMove);

            return !!finder->Find(coordinate);
        }

        bool IsCoordinateInPossibleMoves(const Coordinate& coordinate) const
        {
            const auto it = std::ranges::find(m_possibleMoves, coordinate);

            return it != m_possibleMoves.end();
        }

        bool IsValidMove(const std::shared_ptr<Piece>& piece, const Coordinate& to) const
        {
            if (!piece)
            {
                return false;
            }

            return std::ranges::find(m_possibleMoves, to) != m_possibleMoves.end();
        }
    };
} // namespace Chess
