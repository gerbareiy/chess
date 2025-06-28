#pragma once

#include "logic/Coordinate.h"
#include <boost/signals2.hpp>

#include <memory>
#include <vector>

namespace Chess
{
    class MoveValidator;
    class Piece;
    class PieceDirector;
    class PieceSignalDirector;
    struct Coordinate;

    class Chessboard
    {
    private:
        std::shared_ptr<PieceDirector>      m_director;
        Coordinate                          m_from = Coordinate(0, 0);
        std::shared_ptr<MoveValidator>      m_validator;
        std::vector<std::shared_ptr<Piece>> m_piecesOnBoard;
        Coordinate                          m_to = Coordinate(0, 0);

    private:
        boost::signals2::signal<void()> m_signalChessboardUndated;

    public:
        Chessboard(const std::vector<std::shared_ptr<Piece>>& piecesOnBoard, std::shared_ptr<PieceSignalDirector> signalDirector);

    public:
        Coordinate                            GetFrom() const;
        const std::shared_ptr<MoveValidator>& GetMoveValidator() const;
        const std::shared_ptr<PieceDirector>& GetPieceDirector() const;
        Coordinate                            GetTo() const;

        bool TryInitPiece(const Coordinate& from);

        // Before use this method, you need to InitPiece
        bool TryMovePiece(const Coordinate& to);

    public:
        boost::signals2::connection ConnectChessboardUpdated(const boost::signals2::signal<void()>::slot_type& subscriber);
    };
} // namespace Chess
