module;
#include <boost/signals2.hpp>

#include <memory>
#include <vector>
export module Chess.PieceDirector;
import Chess.CheckChecker;
import Chess.Coordinate;
import Chess.Counts;
import Chess.ePieceColor;
import Chess.King;
import Chess.Pawn;
import Chess.Piece;
import Chess.PieceColorAndType;
import Chess.PieceTakeLocator;
import Chess.Player;
import Chess.Promoter;
import Chess.Promotion;
import Chess.Sizes;

namespace Chess
{
    export class PieceDirector
    {
        std::shared_ptr<Piece>              m_currentPiece;
        std::vector<std::shared_ptr<Piece>> m_eatenPieces;
        bool                                m_isCheck = false;
        std::vector<std::shared_ptr<Piece>> m_piecesOnBoard;
        std::shared_ptr<Promotion>          m_promotion;
        std::shared_ptr<Player>             m_player;

        void Take(int indexOnBoard)
        {
            m_eatenPieces.emplace_back(std::move(m_piecesOnBoard[indexOnBoard]));
            m_piecesOnBoard.erase(m_piecesOnBoard.begin() + indexOnBoard);
        }

    public:
        PieceDirector(const std::vector<std::shared_ptr<Piece>>& piecesOnBoard, const std::shared_ptr<Player>& player)
            : m_piecesOnBoard(piecesOnBoard)
            , m_player(player)
        {
            m_eatenPieces.reserve(MAX_COUNT_ELEMENTS);
            m_promotion = std::make_shared<Promotion>();
        }

        const std::shared_ptr<Piece>& GetCurrentPiece() const { return m_currentPiece; }

        const std::vector<std::shared_ptr<Piece>>& GetEatenPieces() const { return m_eatenPieces; }

        bool GetIsCheck() const { return m_isCheck; }

        const std::vector<std::shared_ptr<Piece>>& GetPiecesOnBoard() const { return m_piecesOnBoard; }

        PieceColorAndType GetPieceColorAndType(const Coordinate& from) const
        {
            if (const auto piece = GetPiece(from))
            {
                return piece->GetColorAndType();
            }
            return {};
        }

        std::shared_ptr<Piece> GetPiece(const Coordinate& from) const
        {
            const auto it = std::ranges::find(m_piecesOnBoard, from, &Piece::GetPosition);
            return it != m_piecesOnBoard.end() ? *it : nullptr;
        }

        void InitCurrentPiece(const Coordinate& from) { m_currentPiece = GetPiece(from); }

        void MovePiece(const Coordinate& to, const boost::signals2::signal<void()>& signalChessboardUndated, const std::shared_ptr<Promoter>& promoter)
        {
            const auto from = PieceTakeLocator::Find(m_currentPiece, m_piecesOnBoard, to);
            const auto it   = std::ranges::find(m_piecesOnBoard, from, &Piece::GetPosition);

            if (it != m_piecesOnBoard.end())
            {
                Take(static_cast<int>(std::distance(m_piecesOnBoard.begin(), it)));
            }

            m_currentPiece->Move(to);
            if (m_player)
            {
                m_player->ChangeColor();
            }

            if (typeid(*m_currentPiece) == typeid(Pawn)
                && (m_currentPiece->GetPosition().rank == 1 && m_currentPiece->GetColorAndType().color == ePieceColor::BLACK
                    || m_currentPiece->GetPosition().rank == CHESSBOARD_SIZE && m_currentPiece->GetColorAndType().color == ePieceColor::WHITE))
            {
                signalChessboardUndated();
                Promotion::PromoteConditionally(std::static_pointer_cast<Pawn>(m_currentPiece), m_piecesOnBoard, promoter);
            }

            auto color = ePieceColor::NONE;
            if (m_currentPiece->GetColorAndType().color == ePieceColor::BLACK)
            {
                color = ePieceColor::WHITE;
            }
            else if (m_currentPiece->GetColorAndType().color == ePieceColor::WHITE)
            {
                color = ePieceColor::BLACK;
            }

            m_isCheck = CheckChecker::IsCheck(color, m_piecesOnBoard);
            if (const auto king = std::dynamic_pointer_cast<King>(m_currentPiece))
            {
                king->SetCheck(m_isCheck);
            }
        }
    };
} // namespace Chess
