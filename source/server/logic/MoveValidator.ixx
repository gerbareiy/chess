module;
#include <algorithm>
#include <memory>
#include <vector>
export module Chess.MoveValidator;
import Chess.Coordinate;
import Chess.CoordinateToPieceBuilder;
import Chess.Counts;
import Chess.MoveCheckerFactory;
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
        }

        void Init()
        {
            RefreshPiecesCanMove();
        }

        std::vector<Coordinate> GetPossibleMoves()
        {
            return m_possibleMoves;
        }

        void RefreshPiecesCanMove()
        {
            ClearPiecesCanMove();
            ClearPossibleMoves();

            std::vector<std::shared_ptr<Piece>> pieces;
            pieces.reserve(MAX_ELEMENTS_COUNT);

            for (const auto& piece : m_piecesOnBoard)
            {
                if (piece->GetColorAndType().color == m_player->GetPlayerColor())
                {
                    const auto moveChecker = MoveCheckerOwner(piece, MoveCheckerFactory::Create(piece));

                    if (moveChecker.GetFilteredMoves(m_piecesOnBoard).size())
                    {
                        pieces.emplace_back(piece);
                    }
                }
            }

            m_piecesCanMove = pieces;
        }

        void RefreshPossibleMoves(const std::shared_ptr<Piece>& piece)
        {
            const auto iter = std::ranges::find(m_piecesCanMove, piece);
            if (iter != m_piecesCanMove.end())
            {
                const auto moveChecker = std::make_shared<MoveCheckerOwner>(piece, MoveCheckerFactory::Create(piece));
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

        size_t GetPiecesCanMoveCount() const
        {
            return m_piecesCanMove.size();
        }

        bool IsCoordinateInPieceCanMove(const Coordinate& coordinate) const
        {
            auto       pieceMap = CoordinateToPieceBuilder::Build(m_piecesCanMove);
            const auto finder   = std::make_unique<PieceFinder>(std::move(pieceMap));
            return !!finder->TryFind(coordinate);
        }

        bool IsCoordinateInPossibleMoves(const Coordinate& coordinate) const
        {
            return std::ranges::contains(m_possibleMoves, coordinate);
        }

        bool IsValidMove(const std::shared_ptr<Piece>& piece, const Coordinate& to) const
        {
            if (piece == nullptr)
            {
                return false;
            }
            return std::ranges::contains(m_possibleMoves, to);
        }
    };
} // namespace Chess
