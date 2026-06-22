module;
#include <boost/signals2.hpp>
#include <memory>
#include <vector>
export module Chess.Chessboard;
import Chess.Coordinate;
import Chess.ePieceColor;
import Chess.MoveValidator;
import Chess.Piece;
import Chess.PieceDirector;
import Chess.Player;
import Chess.Promoter;

namespace Chess
{
    export class Chessboard
    {
        std::shared_ptr<Player>             m_player;
        std::vector<std::shared_ptr<Piece>> m_piecesOnBoard;
        std::shared_ptr<PieceDirector>      m_director;
        std::shared_ptr<MoveValidator>      m_validator;

        Coordinate m_from = { .file = 0, .rank = 0 };
        Coordinate m_to   = { .file = 0, .rank = 0 };

        boost::signals2::signal<void()> m_signalChessboardUpdated;

    public:
        Chessboard(
            const std::shared_ptr<Player>&        player,
            std::vector<std::shared_ptr<Piece>>&& piecesOnBoard,
            std::unique_ptr<PieceDirector>&&      director,
            std::unique_ptr<MoveValidator>&&      validator)
            : m_player(player)
            , m_piecesOnBoard(std::move(piecesOnBoard))
            , m_director(std::move(director))
            , m_validator(std::move(validator))
        {
        }

        void Init()
        {
            m_player->Init();
            m_validator->Init();
        }

        Coordinate GetFrom() const
        {
            return m_from;
        }

        const std::shared_ptr<MoveValidator>& GetMoveValidator() const
        {
            return m_validator;
        }

        const std::shared_ptr<PieceDirector>& GetPieceDirector() const
        {
            return m_director;
        }

        Coordinate GetTo() const
        {
            return m_to;
        }

        bool TrySelectPiece(const Coordinate& from)
        {
            m_from = from;
            m_to   = Coordinate(0, 0);
            m_director->InitCurrentPiece(from);

            if (!m_director->GetCurrentPiece())
            {
                return false;
            }

            m_validator->RefreshPossibleMoves(m_director->GetCurrentPiece());

            if (m_validator->GetPossibleMoves().size() < 1)
            {
                return false;
            }

            m_signalChessboardUpdated();
            return true;
        }

        // Before use this method, you need to InitPiece
        bool TryMovePiece(const Coordinate& to, const std::shared_ptr<Promoter>& promoter)
        {
            m_to = to;

            if (!m_validator->IsValidMove(m_director->GetCurrentPiece(), to))
            {
                return false;
            }

            m_validator->ClearPossibleMoves();
            m_validator->ClearPiecesCanMove();
            m_director->MovePiece(to, m_signalChessboardUpdated, promoter);
            m_validator->RefreshPiecesCanMove();

            m_signalChessboardUpdated();

            return true;
        }

        boost::signals2::connection ConnectChessboardUpdated(const std::function<void()>& subscriber)
        {
            return m_signalChessboardUpdated.connect(subscriber);
        }
    };
} // namespace Chess
