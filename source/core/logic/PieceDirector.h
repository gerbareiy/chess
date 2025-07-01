#pragma once

#include <boost/signals2.hpp>

#include <memory>
#include <vector>

namespace Chess
{
    class LabelShower;
    class PieceSignalDirector;
    class Piece;
    class Promotion;
    struct Coordinate;
    struct PieceColorAndType;

    class PieceDirector
    {
        std::shared_ptr<Piece>               m_currentPiece;
        std::vector<std::shared_ptr<Piece>>  m_eatenPieces;
        std::unique_ptr<LabelShower>         m_inputDisplayer;
        bool                                 m_isCheck = false;
        std::vector<std::shared_ptr<Piece>>  m_piecesOnBoard;
        std::shared_ptr<Promotion>           m_promotion;
        std::shared_ptr<PieceSignalDirector> m_signalDirector;

        void Take(int indexOnBoard);

    public:
        PieceDirector(std::vector<std::shared_ptr<Piece>>& piecesOnBoard, const std::shared_ptr<PieceSignalDirector>& signalDirector);

        const std::shared_ptr<Piece>&              GetCurrentPiece() const;
        const std::vector<std::shared_ptr<Piece>>& GetEatenPieces() const;
        bool                                       GetIsCheck() const;
        const std::vector<std::shared_ptr<Piece>>& GetPiecesOnBoard() const;

        PieceColorAndType      GetPieceColorAndType(const Coordinate& from) const;
        std::shared_ptr<Piece> GetPiece(const Coordinate& from) const;
        void                   InitCurrentPiece(const Coordinate& from);
        void                   MovePiece(const Coordinate& to, const boost::signals2::signal<void()>& signalChessboardUndated);
    };
} // namespace Chess
