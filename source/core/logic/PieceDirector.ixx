module;
#include "core/logic/Coordinate.h"
#include <boost/signals2.hpp>

#include <memory>
#include <vector>
export module Chess.PieceDirector;
import Chess.Piece;
import Chess.PieceColorAndType;
import Chess.PieceSignalDirector;
import Chess.Promotion;

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

        void Take(int indexOnBoard);

    public:
        PieceDirector(const std::vector<std::shared_ptr<Piece>>& piecesOnBoard, const std::shared_ptr<PieceSignalDirector>& signalDirector);

        const std::shared_ptr<Piece>&              GetCurrentPiece() const;
        const std::vector<std::shared_ptr<Piece>>& GetEatenPieces() const;
        bool                                       GetIsCheck() const;
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

        std::shared_ptr<Piece> GetPiece(const Coordinate& from) const;
        void                   InitCurrentPiece(const Coordinate& from);
        void                   MovePiece(const Coordinate& to, const boost::signals2::signal<void()>& signalChessboardUndated);
    };
} // namespace Chess
