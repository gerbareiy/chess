module;
#include <boost/signals2.hpp>

#include <memory>
#include <vector>
export module Chess.PieceDirector;
import Chess.CheckChecker;
import Chess.Coordinate;
import Chess.Counts;
import Chess.ePieceColor;
import Chess.Pawn;
import Chess.Piece;
import Chess.PieceColorAndType;
import Chess.PieceSignalDirector;
import Chess.PieceTakeLocator;
import Chess.Promotion;
import Chess.Sizes;

namespace Chess
{
    export class PieceDirector
    {
        std::shared_ptr<Piece>               m_currentPiece;
        std::vector<std::shared_ptr<Piece>>  m_eatenPieces;
        bool                                 m_isCheck = false;
        std::vector<std::shared_ptr<Piece>>  m_piecesOnBoard;
        std::shared_ptr<Promotion>           m_promotion;
        std::shared_ptr<PieceSignalDirector> m_signalDirector;

        void Take(int indexOnBoard)
        {
            m_eatenPieces.emplace_back(std::move(m_piecesOnBoard[indexOnBoard]));
            m_piecesOnBoard.erase(m_piecesOnBoard.begin() + indexOnBoard);
        }

    public:
        PieceDirector(const std::vector<std::shared_ptr<Piece>>& piecesOnBoard, const std::shared_ptr<PieceSignalDirector>& signalDirector)
            : m_piecesOnBoard(piecesOnBoard)
            , m_signalDirector(signalDirector)
        {
            m_eatenPieces.reserve(MAX_COUNT_ELEMENTS);
            m_promotion = std::make_shared<Promotion>();
        }

        const std::shared_ptr<Piece>& GetCurrentPiece() const
        {
            return m_currentPiece;
        }

        const std::vector<std::shared_ptr<Piece>>& GetEatenPieces() const
        {
            return m_eatenPieces;
        }

        bool GetIsCheck() const
        {
            return m_isCheck;
        }

        const std::vector<std::shared_ptr<Piece>>& GetPiecesOnBoard() const
        {
            return m_piecesOnBoard;
        }

        PieceColorAndType GetPieceColorAndType(const Coordinate& from) const
        {
            const auto piece = GetPiece(from);

            if (!piece)
            {
                return {};
            }

            return piece->GetColorAndType();
        }

        std::shared_ptr<Piece> GetPiece(const Coordinate& from) const
        {
            for (std::shared_ptr<Piece> pieceOnBoard : m_piecesOnBoard)
            {
                if (pieceOnBoard->GetPosition() == from)
                {
                    return pieceOnBoard;
                }
            }

            return nullptr;
        }

        void                   InitCurrentPiece(const Coordinate& from)
        {
            m_currentPiece = GetPiece(from);
        }

        void                   MovePiece(const Coordinate& to, const boost::signals2::signal<void()>& signalChessboardUndated)
        {
            auto fromTake = PieceTakeLocator::Find(m_currentPiece, m_piecesOnBoard, to);

            const auto it = std::ranges::find_if(m_piecesOnBoard,
                                                 [fromTake](const std::shared_ptr<Piece>& current)
                                                 {
                                                     return current->GetPosition() == fromTake;
                                                 });

            if (it != m_piecesOnBoard.end())
            {
                Take(static_cast<int>(std::distance(m_piecesOnBoard.begin(), it)));
            }

            m_currentPiece->Move(to);
            m_signalDirector->Invite();

            if (typeid(*m_currentPiece) == typeid(Pawn)
                && (m_currentPiece->GetPosition().rank == 1 && m_currentPiece->GetColorAndType().color == ePieceColor::BLACK
                    || m_currentPiece->GetPosition().rank == CHESSBOARD_SIZE && m_currentPiece->GetColorAndType().color == ePieceColor::WHITE))
            {
                signalChessboardUndated();
                Promotion::PromoteConditionally(std::static_pointer_cast<Pawn>(m_currentPiece), m_piecesOnBoard);
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
            m_signalDirector->Invite(m_isCheck);
        }
    };
} // namespace Chess
